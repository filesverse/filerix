#include "include/FileSystem/DriveUtils.h"
#include "include/utils/Logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mntent.h>
#include <sys/statvfs.h>
#include <sys/mount.h>
#include <dirent.h>
#include <unistd.h>
#include <regex.h>
#include <errno.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 256

const char *GetMountPoint(const char *device)
{
  logger_info("Attempting to get mount point for device");

  FILE *file = setmntent("/proc/mounts", "r");
  if (!file)
  {
    logger_error("Unable to read /proc/mounts");
    return NULL;
  }

  struct mntent *mnt;
  while ((mnt = getmntent(file)))
  {
    if (strcmp(mnt->mnt_fsname, device) == 0)
    {
      endmntent(file);
      logger_info("Found mount point");
      return mnt->mnt_dir;
    }
  }

  endmntent(file);
  logger_error("Device is not mounted or does not exist");
  return NULL;
}

DriveList GetDrives()
{
  logger_info("Getting list of valid drives with mountable partitions...");

  DriveList drives;
  drives.items = malloc(sizeof(DriveInfo) * 100);
  drives.count = 0;

  if (!drives.items)
  {
    logger_error("Memory allocation failed");
    return drives;
  }

  char hostDrive[MAX_PATH_LENGTH] = {0};
  FILE *file;
  struct mntent *mnt;
  char device[MAX_PATH_LENGTH], mountPoint[MAX_PATH_LENGTH], fsType[MAX_PATH_LENGTH];

  file = setmntent("/proc/mounts", "r");
  if (!file)
  {
    logger_error("Unable to read /proc/mounts");
    free(drives.items);
    drives.items = NULL;
    return drives;
  }

  // Map of mount points
  struct MountInfo
  {
    char mountPoint[MAX_PATH_LENGTH];
    char fsType[MAX_PATH_LENGTH];
  };
  struct MountInfo mountPoints[100];
  int mountCount = 0;

  while ((mnt = getmntent(file)))
  {
    if (strncmp(mnt->mnt_fsname, "/dev/", 5) == 0)
    {
      strncpy(mountPoints[mountCount].mountPoint, mnt->mnt_dir, MAX_PATH_LENGTH);
      strncpy(mountPoints[mountCount].fsType, mnt->mnt_type, MAX_PATH_LENGTH);

      if (strcmp(mnt->mnt_dir, "/") == 0)
      {
        strncpy(hostDrive, mnt->mnt_fsname, MAX_PATH_LENGTH);
      }

      mountCount++;
    }
  }
  endmntent(file);

  // Regex for partitions and drives
  regex_t driveRegex, partitionRegex;
  regcomp(&driveRegex, "^/dev/(sd[a-z]+|nvme[0-9]+n[0-9]+)$", REG_EXTENDED);
  regcomp(&partitionRegex, "^/dev/(sd[a-z]+[0-9]+|nvme[0-9]+n[0-9]+p[0-9]+)$", REG_EXTENDED);

  struct dirent *entry;
  DIR *dir = opendir("/sys/class/block");
  if (!dir)
  {
    logger_error("Unable to open /sys/class/block");
    free(drives.items);
    drives.items = NULL;
    return drives;
  }

  while ((entry = readdir(dir)))
  {
    char devicePath[MAX_PATH_LENGTH];
    snprintf(devicePath, MAX_PATH_LENGTH, "/dev/%s", entry->d_name);

    char ueventFile[MAX_PATH_LENGTH];
    snprintf(ueventFile, MAX_PATH_LENGTH, "/sys/class/block/%s/device/uevent", entry->d_name);
    FILE *ueventStream = fopen(ueventFile, "r");

    int isUsb = 0;
    if (ueventStream)
    {
      char line[MAX_PATH_LENGTH];
      while (fgets(line, sizeof(line), ueventStream))
      {
        if (strstr(line, "DEVTYPE=disk") && strstr(line, "ID_BUS=usb"))
        {
          logger_info("Skipping USB device: %s", devicePath);
          isUsb = 1;
          break;
        }
      }
      fclose(ueventStream);
    }

    if (isUsb)
      continue;

    if (regexec(&partitionRegex, devicePath, 0, NULL, 0) == 0)
    {
      DriveInfo drive;
      snprintf(drive.device, MAX_PATH_LENGTH, "%s", devicePath);
      snprintf(drive.partition, MAX_PATH_LENGTH, "%s", devicePath);
      drive.unmountable = 1;

      int mounted = 0;
      for (int i = 0; i < mountCount; i++)
      {
        if (strcmp(devicePath, mountPoints[i].mountPoint) == 0)
        {
          mounted = 1;
          snprintf(drive.status, MAX_PATH_LENGTH, "mounted");
          snprintf(drive.mountPoint, MAX_PATH_LENGTH, "%s", mountPoints[i].mountPoint);
          snprintf(drive.fsType, MAX_PATH_LENGTH, "%s", mountPoints[i].fsType);

          if ((strcmp(drive.fsType, "vfat") == 0 && strcmp(drive.mountPoint, "/boot/efi") == 0) ||
              strcmp(drive.mountPoint, "/boot") == 0 ||
              strcmp(drive.mountPoint, "/home") == 0)
          {
            logger_info("Skipping partition: %s (EFI, /boot, or /home)", drive.device);
            goto continue_loop;
          }

          logger_info("Drive %s is mounted at %s", drive.device, drive.mountPoint);
          break;
        }
      }

      if (!mounted)
      {
        snprintf(drive.status, MAX_PATH_LENGTH, "unmounted");
        snprintf(drive.mountPoint, MAX_PATH_LENGTH, "");
        logger_info("Drive %s is unmounted.", drive.device);
      }

      if (strcmp(drive.device, hostDrive) == 0)
      {
        drive.unmountable = 0;
      }

      drives.items[drives.count++] = drive;
    }

  continue_loop:
    continue;
  }

  closedir(dir);
  regfree(&driveRegex);
  regfree(&partitionRegex);

  if (strlen(hostDrive) > 0)
  {
    logger_info("Adding host drive: %s", hostDrive);
    DriveInfo hostDriveInfo;
    snprintf(hostDriveInfo.device, MAX_PATH_LENGTH, "%s", hostDrive);
    snprintf(hostDriveInfo.partition, MAX_PATH_LENGTH, "%s", hostDrive);
    snprintf(hostDriveInfo.mountPoint, MAX_PATH_LENGTH, "/");
    snprintf(hostDriveInfo.status, MAX_PATH_LENGTH, "mounted");

    for (int i = 0; i < mountCount; i++)
    {
      if (strcmp(hostDrive, mountPoints[i].mountPoint) == 0)
      {
        snprintf(hostDriveInfo.fsType, MAX_PATH_LENGTH, "%s", mountPoints[i].fsType);
        break;
      }
    }
    hostDriveInfo.unmountable = 0;
    drives.items[drives.count++] = hostDriveInfo;
  }

  logger_info("Drives sorted.");
  return drives;
}

bool MountDrive(const char *device)
{
  logger_info("Attempting to mount drive");

  char command[512];
  snprintf(command, sizeof(command), "pkexec mount %s", device);

  int ret = system(command);
  if (ret != 0)
  {
    logger_error("Failed to mount drive");
    return false;
  }

  logger_info("Drive mounted successfully");
  return true;
}

bool UnmountDrive(const char *device)
{
  logger_info("Attempting to unmount drive");

  char command[512];
  snprintf(command, sizeof(command), "pkexec umount %s", device);

  int ret = system(command);
  if (ret != 0)
  {
    logger_error("Failed to unmount drive");
    return false;
  }

  logger_info("Drive unmounted successfully");
  return true;
}

const char *GetDeviceLabelOrUUID(const char *device)
{
  static char label_or_uuid[256];
  struct stat stat_buf;

  DIR *dir = opendir("/dev/disk/by-label");
  if (dir)
  {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
      char label_path[512];
      snprintf(label_path, sizeof(label_path), "/dev/disk/by-label/%s", entry->d_name);

      if (lstat(label_path, &stat_buf) == 0)
      {
        char resolved_path[512];
        if (realpath(label_path, resolved_path) != NULL && strcmp(resolved_path, device) == 0)
        {
          snprintf(label_or_uuid, sizeof(label_or_uuid), "%s", entry->d_name);
          closedir(dir);
          return label_or_uuid;
        }
      }
    }
    closedir(dir);
  }

  dir = opendir("/dev/disk/by-uuid");
  if (dir)
  {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
      char uuid_path[512];
      snprintf(uuid_path, sizeof(uuid_path), "/dev/disk/by-uuid/%s", entry->d_name);

      if (lstat(uuid_path, &stat_buf) == 0)
      {
        char resolved_path[512];
        if (realpath(uuid_path, resolved_path) != NULL && strcmp(resolved_path, device) == 0)
        {
          snprintf(label_or_uuid, sizeof(label_or_uuid), "%s", entry->d_name);
          closedir(dir);
          return label_or_uuid;
        }
      }
    }
    closedir(dir);
  }

  return NULL;
}

DriveUsage GetDriveUsage(const char *drive)
{
  logger_info("Getting drive usage");

  DriveUsage usage = {0};

  const char *mountPoint = GetMountPoint(drive);
  if (mountPoint)
  {
    struct statvfs stat;
    if (statvfs(mountPoint, &stat) == 0)
    {
      usage.total = stat.f_blocks * stat.f_frsize;
      usage.used = (stat.f_blocks - stat.f_bfree) * stat.f_frsize;
    }
    else
    {
      logger_error("Unable to get drive usage");
    }
  }

  return usage;
}
