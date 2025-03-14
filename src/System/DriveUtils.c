#include "System/DriveUtils.h"
#include "Utils/Logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mntent.h>
#include <sys/statvfs.h>
#include <sys/mount.h>
#include <unistd.h>
#include <dirent.h>
#include <dbus/dbus.h>

const char *DriveUtils_GetMountPoint(const char *device)
{
  Logger_Info("[DriveUtils] Attempting to get mount point for device: %s", device);

  FILE *file = setmntent("/proc/mounts", "r");
  if (!file)
  {
    Logger_Error("[DriveUtils] Unable to read /proc/mounts");
    return NULL;
  }

  struct mntent *mnt;
  while ((mnt = getmntent(file)))
  {
    if (strcmp(mnt->mnt_fsname, device) == 0)
    {
      endmntent(file);
      Logger_Info("[DriveUtils] Mount point for %s is %s", device, mnt->mnt_dir);
      return strdup(mnt->mnt_dir);
    }
  }

  endmntent(file);
  Logger_Error("[DriveUtils] Device %s is not mounted or does not exist.", device);
  return NULL;
}

DriveList DriveUtils_GetDrives()
{
  Logger_Info("[DriveUtils] Getting list of valid drives with mountable partitions...");

  DriveList driveList;
  driveList.drives = NULL;
  driveList.count = 0;

  FILE *file = setmntent("/proc/mounts", "r");
  if (!file)
  {
    Logger_Error("[DriveUtils] Unable to read /proc/mounts");
    return driveList;
  }

  struct mntent *mnt;
  while ((mnt = getmntent(file)))
  {
    if (strncmp(mnt->mnt_fsname, "/dev/", 5) == 0)
    {
      driveList.drives = realloc(driveList.drives, (driveList.count + 1) * sizeof(DriveInfo));
      strcpy(driveList.drives[driveList.count].device, mnt->mnt_fsname);
      strcpy(driveList.drives[driveList.count].mountPoint, mnt->mnt_dir);
      strcpy(driveList.drives[driveList.count].fsType, mnt->mnt_type);
      strcpy(driveList.drives[driveList.count].status, "mounted");
      driveList.drives[driveList.count].unmountable = true;
      driveList.count++;
    }
  }

  endmntent(file);
  Logger_Info("[DriveUtils] Found %zu drives.", driveList.count);
  return driveList;
}

DriveUsage DriveUtils_GetDriveUsage(const char *drive)
{
  Logger_Info("[DriveUtils] Getting drive usage for: %s", drive);

  DriveUsage usage = {0, 0};
  const char *mountPoint = DriveUtils_GetMountPoint(drive);
  if (!mountPoint)
  {
    Logger_Error("[DriveUtils] Unable to determine mount point for %s", drive);
    return usage;
  }

  struct statvfs stat;
  if (statvfs(mountPoint, &stat) != 0)
  {
    Logger_Error("[DriveUtils] Unable to get drive information for %s", drive);
    return usage;
  }

  usage.total = stat.f_blocks * stat.f_frsize;
  usage.used = (stat.f_blocks - stat.f_bfree) * stat.f_frsize;

  Logger_Info("[DriveUtils] Drive usage for %s: %llu / %llu", drive, usage.used, usage.total);
  return usage;
}

const char *DriveUtils_GetDeviceLabelOrUUID(const char *pathOrDevice)
{
  Logger_Info("[DriveUtils] Getting label or UUID for: %s", pathOrDevice);

  struct dirent *entry;
  DIR *dir = opendir("/dev/disk/by-label");
  if (dir)
  {
    while ((entry = readdir(dir)))
    {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;

      char fullPath[512];
      snprintf(fullPath, sizeof(fullPath), "/dev/disk/by-label/%s", entry->d_name);

      if (realpath(fullPath, NULL) && strcmp(realpath(fullPath, NULL), pathOrDevice) == 0)
      {
        Logger_Info("[DriveUtils] Found label for %s: %s", pathOrDevice, entry->d_name);
        closedir(dir);
        return strdup(entry->d_name);
      }
    }
    closedir(dir);
  }

  dir = opendir("/dev/disk/by-uuid");
  if (dir)
  {
    while ((entry = readdir(dir)))
    {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;

      char fullPath[512];
      snprintf(fullPath, sizeof(fullPath), "/dev/disk/by-uuid/%s", entry->d_name);

      if (realpath(fullPath, NULL) && strcmp(realpath(fullPath, NULL), pathOrDevice) == 0)
      {
        Logger_Info("[DriveUtils] Found UUID for %s: %s", pathOrDevice, entry->d_name);
        closedir(dir);
        return strdup(entry->d_name);
      }
    }
    closedir(dir);
  }

  Logger_Warn("[DriveUtils] No label or UUID found for: %s", pathOrDevice);
  return NULL;
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
