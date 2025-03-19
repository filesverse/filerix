#include "System/DriveUtils.h"
#include "Utils/Logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mntent.h>
#include <sys/statvfs.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <dbus/dbus.h>
#include <regex.h>

DriveList DriveUtils_GetDrives()
{
  DriveList list;
  list.count = 0;
  list.devices = NULL;

  regex_t partitionRegex;
  if (regcomp(&partitionRegex, "^/dev/(sd[a-z]+[0-9]+|nvme[0-9]+n[0-9]+p[0-9]+)$", REG_EXTENDED) != 0)
    return list;

  FILE *mntFile = setmntent("/proc/mounts", "r");
  struct mntent *mnt;

  while (mntFile && (mnt = getmntent(mntFile)) != NULL)
  {
    if (strncmp(mnt->mnt_fsname, "/dev/", 5) == 0 &&
        regexec(&partitionRegex, mnt->mnt_fsname, 0, NULL, 0) == 0 &&
        strcmp(mnt->mnt_dir, "/boot") != 0 &&
        strcmp(mnt->mnt_dir, "/boot/efi") != 0 &&
        strcmp(mnt->mnt_dir, "/home") != 0)
    {
      char **temp = (char **)realloc(list.devices, (list.count + 1) * sizeof(char *));
      if (!temp)
        continue;

      list.devices = temp;
      list.devices[list.count] = strdup(mnt->mnt_fsname);
      if (!list.devices[list.count])
        continue;

      list.count++;
    }
  }
  if (mntFile)
    endmntent(mntFile);

  DIR *dir = opendir("/sys/class/block");
  struct dirent *entry;

  while (dir && (entry = readdir(dir)) != NULL)
  {
    if (entry->d_name[0] == '.')
      continue;

    char *devicePath;
    if (asprintf(&devicePath, "/dev/%s", entry->d_name) == -1)
      continue;

    if (regexec(&partitionRegex, devicePath, 0, NULL, 0) != 0)
    {
      free(devicePath);
      continue;
    }

    int found = 0;
    for (size_t i = 0; i < list.count; i++)
    {
      if (strcmp(list.devices[i], devicePath) == 0)
      {
        found = 1;
        break;
      }
    }

    if (!found)
    {
      int isBoot = 0;
      FILE *bootMntFile = setmntent("/etc/mtab", "r");
      struct mntent *bootMnt;

      while (bootMntFile && (bootMnt = getmntent(bootMntFile)) != NULL)
      {
        if (strcmp(bootMnt->mnt_fsname, devicePath) == 0 &&
            (strcmp(bootMnt->mnt_dir, "/boot") == 0 ||
             strcmp(bootMnt->mnt_dir, "/boot/efi") == 0 ||
             strcmp(bootMnt->mnt_dir, "/home") == 0))
        {
          isBoot = 1;
          break;
        }
      }
      if (bootMntFile)
        endmntent(bootMntFile);

      if (!isBoot)
      {
        char *command;
        if (asprintf(&command, "lsblk -n -o FSTYPE %s 2>/dev/null", devicePath) == -1)
        {
          free(devicePath);
          continue;
        }

        FILE *fp = popen(command, "r");
        free(command);

        if (fp)
        {
          char buffer[64] = {0};
          if (fgets(buffer, sizeof(buffer), fp))
            buffer[strcspn(buffer, "\n")] = 0;

          pclose(fp);

          if (strlen(buffer) > 0)
          {
            char **temp = (char **)realloc(list.devices, (list.count + 1) * sizeof(char *));
            if (!temp)
            {
              free(devicePath);
              continue;
            }

            list.devices = temp;
            list.devices[list.count] = devicePath;
            list.count++;
          }
          else
          {
            free(devicePath);
          }
        }
        else
        {
          free(devicePath);
        }
      }
      else
      {
        free(devicePath);
      }
    }
    else
    {
      free(devicePath);
    }
  }
  if (dir)
    closedir(dir);

  regfree(&partitionRegex);
  return list;
}

DriveInfo DriveUtils_GetDriveInfo(const char *device)
{
  Logger_Info("[DriveUtils] Getting full details for device: %s", device);

  DriveInfo drive;
  memset(&drive, 0, sizeof(DriveInfo));
  snprintf(drive.device, sizeof(drive.device), "%s", device);
  snprintf(drive.status, sizeof(drive.status), "unmounted");

  FILE *file = setmntent("/proc/mounts", "r");
  if (file)
  {
    struct mntent *mnt;
    while ((mnt = getmntent(file)))
    {
      if (strcmp(mnt->mnt_fsname, device) == 0)
      {
        snprintf(drive.mountPoint, sizeof(drive.mountPoint), "%s", mnt->mnt_dir);
        snprintf(drive.fsType, sizeof(drive.fsType), "%s", mnt->mnt_type);
        drive.readOnly = (hasmntopt(mnt, "ro") != NULL);
        snprintf(drive.status, sizeof(drive.status), "mounted");
        break;
      }
    }
    endmntent(file);
  }

  struct statvfs stat;
  if (strlen(drive.mountPoint) > 0 && statvfs(drive.mountPoint, &stat) == 0)
  {
    drive.total = stat.f_blocks * stat.f_frsize;
    drive.used = (stat.f_blocks - stat.f_bfree) * stat.f_frsize;
    drive.free = stat.f_bavail * stat.f_frsize;
  }

  struct dirent *entry;
  DIR *dir = opendir("/dev/disk/by-label");
  if (dir)
  {
    while ((entry = readdir(dir)))
    {
      if (entry->d_name[0] == '.')
        continue;

      char fullPath[512], resolvedPath[512];
      snprintf(fullPath, sizeof(fullPath), "/dev/disk/by-label/%s", entry->d_name);

      if (realpath(fullPath, resolvedPath) && strcmp(resolvedPath, drive.device) == 0)
      {
        snprintf(drive.label, sizeof(drive.label), "%s", entry->d_name);
        break;
      }
    }
    closedir(dir);
  }

  dir = opendir("/dev/disk/by-uuid");
  if (dir)
  {
    while ((entry = readdir(dir)))
    {
      if (entry->d_name[0] == '.')
        continue;

      char fullPath[512], resolvedPath[512];
      snprintf(fullPath, sizeof(fullPath), "/dev/disk/by-uuid/%s", entry->d_name);

      if (realpath(fullPath, resolvedPath) && strcmp(resolvedPath, drive.device) == 0)
      {
        snprintf(drive.uuid, sizeof(drive.uuid), "%s", entry->d_name);
        break;
      }
    }
    closedir(dir);
  }

  if (strlen(drive.label) == 0)
    snprintf(drive.label, sizeof(drive.label), "Unknown");

  if (strlen(drive.uuid) == 0)
    snprintf(drive.uuid, sizeof(drive.uuid), "Unknown");

  char removablePath[512];
  snprintf(removablePath, sizeof(removablePath), "/sys/block/%s/removable", device + 5);
  FILE *removableFile = fopen(removablePath, "r");
  if (removableFile)
  {
    int removable = fgetc(removableFile);
    drive.removable = (removable == '1');
    fclose(removableFile);
  }

  char usbCheckPath[512];
  snprintf(usbCheckPath, sizeof(usbCheckPath), "/sys/block/%s/device/uevent", device + 5);
  FILE *usbFile = fopen(usbCheckPath, "r");
  if (usbFile)
  {
    char line[256];
    while (fgets(line, sizeof(line), usbFile))
    {
      if (strstr(line, "DRIVER=usb-storage") != NULL)
      {
        drive.removable = 1;
        break;
      }
    }
    fclose(usbFile);
  }

  if (strlen(drive.fsType) == 0)
  {
    char command[512], buffer[64] = {0};
    snprintf(command, sizeof(command), "lsblk -n -o FSTYPE %s 2>/dev/null", device);

    FILE *fp = popen(command, "r");
    if (fp)
    {
      if (fgets(buffer, sizeof(buffer), fp))
      {
        buffer[strcspn(buffer, "\n")] = 0;
        snprintf(drive.fsType, sizeof(drive.fsType), "%s", buffer);
      }
      pclose(fp);
    }
  }

  char roPath[512];
  snprintf(roPath, sizeof(roPath), "/sys/block/%s/ro", device + 5);
  FILE *roFile = fopen(roPath, "r");
  if (roFile)
  {
    int ro = fgetc(roFile);
    drive.readOnly = (ro == '1');
    fclose(roFile);
  }

  Logger_Info("[DriveUtils] Retrieved details for %s: Mount=%s, FS=%s, Label=%s, UUID=%s, Total=%llu, Used=%llu, Free=%llu, ReadOnly=%s, Removable=%s, Status=%s",
              drive.device, drive.mountPoint, drive.fsType, drive.label, drive.uuid,
              drive.total, drive.used, drive.free,
              drive.readOnly ? "Yes" : "No",
              drive.removable ? "Yes" : "No",
              drive.status);

  return drive;
}

bool DriveUtils_MountDrive(const char *device)
{
  Logger_Info("[DriveUtils] Attempting to mount drive: %s", device);

  DBusError err;
  dbus_error_init(&err);

  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (!conn)
  {
    Logger_Error("[DriveUtils] Failed to connect to D-Bus: %s", err.message);
    dbus_error_free(&err);
    return false;
  }

  char dbus_path[512];
  snprintf(dbus_path, sizeof(dbus_path), "/org/freedesktop/UDisks2/block_devices/%s", device + 5);

  Logger_Info("[DriveUtils] Final D-Bus Path: %s", dbus_path);

  DBusMessage *msg = dbus_message_new_method_call(
      "org.freedesktop.UDisks2",
      dbus_path,
      "org.freedesktop.UDisks2.Filesystem",
      "Mount");

  if (!msg)
  {
    Logger_Error("[DriveUtils] Failed to create D-Bus message.");
    return false;
  }

  DBusMessageIter args;
  dbus_message_iter_init_append(msg, &args);
  DBusMessageIter dict;
  dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "{sv}", &dict);
  dbus_message_iter_close_container(&args, &dict);

  DBusMessage *reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
  dbus_message_unref(msg);

  if (!reply)
  {
    Logger_Error("[DriveUtils] Mount Failed: %s", err.message);
    dbus_error_free(&err);
    return false;
  }

  dbus_message_unref(reply);
  Logger_Info("[DriveUtils] Drive %s mounted successfully.", device);
  return true;
}

bool DriveUtils_UnmountDrive(const char *device)
{
  Logger_Info("[DriveUtils] Attempting to unmount drive: %s", device);

  DBusError err;
  dbus_error_init(&err);

  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (!conn)
  {
    Logger_Error("[DriveUtils] Failed to connect to D-Bus: %s", err.message);
    dbus_error_free(&err);
    return false;
  }

  char dbus_path[512];
  snprintf(dbus_path, sizeof(dbus_path), "/org/freedesktop/UDisks2/block_devices/%s", device + 5);

  Logger_Info("[DriveUtils] Final D-Bus Path: %s", dbus_path);

  DBusMessage *msg = dbus_message_new_method_call(
      "org.freedesktop.UDisks2",
      dbus_path,
      "org.freedesktop.UDisks2.Filesystem",
      "Unmount");

  if (!msg)
  {
    Logger_Error("[DriveUtils] Failed to create D-Bus message.");
    return false;
  }

  DBusMessage *reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
  dbus_message_unref(msg);

  if (!reply)
  {
    Logger_Error("[DriveUtils] Unmount Failed: %s", err.message);
    dbus_error_free(&err);
    return false;
  }

  dbus_message_unref(reply);
  Logger_Info("[DriveUtils] Drive %s unmounted successfully.", device);
  return true;
}
