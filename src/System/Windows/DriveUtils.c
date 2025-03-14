#include "System/DriveUtils.h"
#include "Utils/Logger.h"
#include <windows.h>
#include <fileapi.h>
#include <shlwapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "shlwapi.lib")

DriveUsage DriveUtils_GetDriveUsage(const char *drive)
{
  Logger_Info("[DriveUtils] Getting drive usage for: ");
  Logger_Info(drive);

  ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
  if (!GetDiskFreeSpaceExA(drive, &freeBytesAvailable, &totalBytes, &totalFreeBytes))
  {
    Logger_Error("[DriveUtils] Unable to get drive usage for ");
    Logger_Error(drive);
    DriveUsage usage = {0, 0};
    return usage;
  }

  DriveUsage usage;
  usage.used = totalBytes.QuadPart - totalFreeBytes.QuadPart;
  usage.total = totalBytes.QuadPart;
  return usage;
}

const char *DriveUtils_GetMountPoint(const char *device)
{
  size_t len = strlen(device) + 2;
  char *mountPoint = (char *)malloc(len);
  if (mountPoint)
  {
    snprintf(mountPoint, len, "%s\\", device);
  }
  return mountPoint;
}

bool DriveUtils_MountDrive(const char *device)
{
  (void)device;
  Logger_Info("[DriveUtils] Mounting is not required on Windows.");
  return true;
}

bool DriveUtils_UnmountDrive(const char *device)
{
  (void)device;
  Logger_Info("[DriveUtils] Unmounting is not required on Windows.");
  return true;
}

const char *DriveUtils_GetDeviceLabelOrUUID(const char *device)
{
  char volumeLabel[MAX_PATH] = {0};
  char fileSystemName[MAX_PATH] = {0};
  DWORD serialNumber = 0;

  char path[MAX_PATH];
  snprintf(path, sizeof(path), "%s\\", device);

  if (GetVolumeInformationA(path, volumeLabel, sizeof(volumeLabel),
                            &serialNumber, NULL, NULL, fileSystemName, sizeof(fileSystemName)))
  {
    return _strdup(volumeLabel);
  }

  Logger_Warn("[DriveUtils] No label found for ");
  Logger_Warn(device);
  return NULL;
}

DriveList DriveUtils_GetDrives()
{
  Logger_Info("[DriveUtils] Getting list of valid drives...");

  DriveList driveList;
  driveList.count = 0;
  driveList.drives = NULL;

  char driveStrings[256];
  if (GetLogicalDriveStringsA(sizeof(driveStrings), driveStrings) == 0)
  {
    Logger_Error("[DriveUtils] Unable to get drives.");
    return driveList;
  }

  int count = 0;
  char *drive = driveStrings;

  while (*drive)
  {
    count++;
    drive += strlen(drive) + 1;
  }

  driveList.drives = (DriveInfo *)malloc(count * sizeof(DriveInfo));
  if (!driveList.drives)
  {
    Logger_Error("[DriveUtils] Memory allocation failed.");
    return driveList;
  }

  drive = driveStrings;
  for (int i = 0; i < count; i++)
  {
    DriveInfo *info = &driveList.drives[i];
    memset(info, 0, sizeof(DriveInfo));

    strcpy(info->device, drive);
    strcpy(info->mountPoint, drive);

    UINT driveType = GetDriveTypeA(drive);
    switch (driveType)
    {
    case DRIVE_FIXED:
      strcpy(info->status, "mounted");
      strcpy(info->fsType, "NTFS/FAT32");
      info->unmountable = false;
      break;
    case DRIVE_REMOVABLE:
      strcpy(info->status, "mounted");
      strcpy(info->fsType, "NTFS/FAT32");
      info->unmountable = true;
      break;
    case DRIVE_CDROM:
      strcpy(info->status, "mounted");
      strcpy(info->fsType, "ISO9660");
      info->unmountable = true;
      break;
    case DRIVE_REMOTE:
      strcpy(info->status, "mounted");
      strcpy(info->fsType, "Network");
      info->unmountable = false;
      break;
    default:
      strcpy(info->status, "unknown");
      strcpy(info->fsType, "unknown");
      info->unmountable = false;
      break;
    }

    drive += strlen(drive) + 1;
  }

  driveList.count = count;
  return driveList;
}
