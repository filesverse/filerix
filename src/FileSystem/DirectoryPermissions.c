#include "FileSystem/DirectoryPermissions.h"
#include "Utils/Logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dbus/dbus.h>
#include <stdint.h>

bool MountFuseOverlay(const char *target)
{
  char mountPoint[256];
  snprintf(mountPoint, sizeof(mountPoint), "/tmp/filerix_mount_%d", geteuid());

  Logger_Info("[DirectoryPermissions] Mounting FUSE overlay for: %s", target);

  char cmd[512];
  snprintf(cmd, sizeof(cmd),
           "mkdir -p %s && pkexec bindfs --force-user=%d --perms=a+r %s %s",
           mountPoint, geteuid(), target, mountPoint);

  int status = system(cmd);
  if (status != 0)
  {
    Logger_Error("[DirectoryPermissions] Failed to mount FUSE filesystem.");
    return false;
  }

  Logger_Info("[DirectoryPermissions] Mounted FUSE overlay at: %s", mountPoint);
  return true;
}

bool UnmountFuse()
{
  char mountPoint[256];
  snprintf(mountPoint, sizeof(mountPoint), "/tmp/filerix_mount_%d", geteuid());

  char cmd[256];
  snprintf(cmd, sizeof(cmd), "fusermount -u %s", mountPoint);

  int status = system(cmd);
  if (status != 0)
  {
    Logger_Warn("[DirectoryPermissions] Failed to unmount FUSE filesystem.");
    return false;
  }

  Logger_Info("[DirectoryPermissions] FUSE filesystem unmounted.");
  return true;
}

bool HasDirectAccess(const char *path)
{
  struct stat info;
  if (stat(path, &info) != 0)
  {
    Logger_Warn("[DirectoryPermissions] Cannot access: %s", path);
    return false;
  }

  uid_t uid = geteuid();
  gid_t gid = getegid();

  if ((uid == info.st_uid && (info.st_mode & S_IRUSR)) ||
      (gid == info.st_gid && (info.st_mode & S_IRGRP)) ||
      (info.st_mode & S_IROTH))
  {
    return true;
  }

  Logger_Warn("[DirectoryPermissions] No permission for: %s", path);
  return false;
}

bool RequestAccessViaDBus(const char *path)
{
  Logger_Info("[DirectoryPermissions] Requesting elevated access via D-Bus...");

  DBusError err;
  dbus_error_init(&err);

  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (!conn)
  {
    Logger_Error("[DirectoryPermissions] Failed to connect to D-Bus: %s", err.message);
    dbus_error_free(&err);
    return false;
  }

  DBusMessage *msg = dbus_message_new_method_call(
      "org.freedesktop.PolicyKit1",
      "/org/freedesktop/PolicyKit1/Authority",
      "org.freedesktop.PolicyKit1.Authority",
      "CheckAuthorization");

  if (!msg)
  {
    Logger_Error("[DirectoryPermissions] Failed to create D-Bus message.");
    return false;
  }

  DBusMessageIter args, struct_iter, dict_iter, entry_iter, variant_iter;
  dbus_message_iter_init_append(msg, &args);

  dbus_message_iter_open_container(&args, DBUS_TYPE_STRUCT, NULL, &struct_iter);
  const char *subject_kind = "unix-process";
  dbus_message_iter_append_basic(&struct_iter, DBUS_TYPE_STRING, &subject_kind);
  dbus_message_iter_open_container(&struct_iter, DBUS_TYPE_ARRAY, "{sv}", &dict_iter);

  const char *key_pid = "pid";
  uint32_t pid = getpid();
  dbus_message_iter_open_container(&dict_iter, DBUS_TYPE_DICT_ENTRY, NULL, &entry_iter);
  dbus_message_iter_append_basic(&entry_iter, DBUS_TYPE_STRING, &key_pid);
  dbus_message_iter_open_container(&entry_iter, DBUS_TYPE_VARIANT, "u", &variant_iter);
  dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_UINT32, &pid);
  dbus_message_iter_close_container(&entry_iter, &variant_iter);
  dbus_message_iter_close_container(&dict_iter, &entry_iter);

  uint64_t start_time = 0;
  char statPath[256];
  snprintf(statPath, sizeof(statPath), "/proc/%d/stat", pid);
  FILE *statFile = fopen(statPath, "r");
  if (statFile)
  {
    char statLine[1024];
    if (fgets(statLine, sizeof(statLine), statFile))
    {
      char *token = strtok(statLine, " ");
      for (int i = 1; token != NULL && i < 22; ++i)
      {
        token = strtok(NULL, " ");
      }
      if (token)
      {
        start_time = strtoull(token, NULL, 10);
      }
    }
    fclose(statFile);
  }
  else
  {
    Logger_Error("[DirectoryPermissions] Failed to read /proc/%d/stat", pid);
  }

  const char *key_start_time = "start-time";
  dbus_message_iter_open_container(&dict_iter, DBUS_TYPE_DICT_ENTRY, NULL, &entry_iter);
  dbus_message_iter_append_basic(&entry_iter, DBUS_TYPE_STRING, &key_start_time);
  dbus_message_iter_open_container(&entry_iter, DBUS_TYPE_VARIANT, "t", &variant_iter);
  dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_UINT64, &start_time);
  dbus_message_iter_close_container(&entry_iter, &variant_iter);
  dbus_message_iter_close_container(&dict_iter, &entry_iter);

  dbus_message_iter_close_container(&struct_iter, &dict_iter);
  dbus_message_iter_close_container(&args, &struct_iter);

  const char *action = "org.filerix.read";
  dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &action);

  dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "{ss}", &dict_iter);
  dbus_message_iter_close_container(&args, &dict_iter);

  uint32_t flags = 1;
  dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &flags);

  const char *interactive = "none";
  dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &interactive);

  DBusMessage *reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
  dbus_message_unref(msg);

  if (!reply)
  {
    Logger_Error("[DirectoryPermissions] D-Bus request failed: %s", err.message);
    dbus_error_free(&err);
    return false;
  }

  DBusMessageIter reply_iter;
  dbus_message_iter_init(reply, &reply_iter);
  dbus_bool_t granted;
  dbus_message_iter_get_basic(&reply_iter, &granted);
  dbus_message_unref(reply);

  return granted;
}

bool HasAccess(const char *path)
{
  if (HasDirectAccess(path))
  {
    Logger_Info("[DirectoryPermissions] Direct access granted for: %s", path);
    return true;
  }

  Logger_Info("[DirectoryPermissions] Direct access denied, trying FUSE...");
  if (MountFuseOverlay(path))
  {
    Logger_Info("[DirectoryPermissions] FUSE overlay mounted, access granted.");
    return true;
  }

  Logger_Info("[DirectoryPermissions] Trying D-Bus authentication...");
  bool accessGranted = RequestAccessViaDBus(path);

  if (!accessGranted)
  {
    UnmountFuse();
  }

  return accessGranted;
}
