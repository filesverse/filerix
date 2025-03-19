#include "System/DriveUtils.h"
#include "Utils/Logger.h"
#include <windows.h>
#include <fileapi.h>
#include <shlwapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "shlwapi.lib")

DriveList DriveUtils_GetDrives()
{
  Logger_Info("[DriveUtils] Getting list of drives...");

  DriveList list;
  list.count = 0;
  list.devices = NULL;

  char driveStrings[256];
  if (GetLogicalDriveStringsA(sizeof(driveStrings), driveStrings) == 0)
  {
    Logger_Error("[DriveUtils] Unable to get drives.");
    return list;
  }

  char *drive = driveStrings;
  while (*drive)
  {
    UINT type = GetDriveTypeA(drive);
    if (type == DRIVE_FIXED || type == DRIVE_REMOVABLE)
    {
      char **temp = (char **)realloc(list.devices, (list.count + 1) * sizeof(char *));
      if (!temp)
        continue;

      list.devices = temp;
      list.devices[list.count] = _strdup(drive);
      list.count++;
    }
    drive += strlen(drive) + 1;
  }

  return list;
}

DriveInfo DriveUtils_GetDriveInfo(const char *device)
{
  Logger_Info("[DriveUtils] Getting details for device: %s", device);

  DriveInfo drive;
  memset(&drive, 0, sizeof(DriveInfo));
  snprintf(drive.device, sizeof(drive.device), "%s", device);
  snprintf(drive.status, sizeof(drive.status), "mounted");

  UINT type = GetDriveTypeA(device);
  if (type == DRIVE_REMOVABLE)
    drive.removable = true;
  else if (type == DRIVE_FIXED)
    drive.removable = false;
  else
  {
    Logger_Warn("[DriveUtils] Ignoring non-removable or network drive: %s", device);
    return drive;
  }

  ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
  if (GetDiskFreeSpaceExA(device, &freeBytesAvailable, &totalBytes, &totalFreeBytes))
  {
    drive.total = totalBytes.QuadPart;
    drive.used = totalBytes.QuadPart - totalFreeBytes.QuadPart;
    drive.free = totalFreeBytes.QuadPart;
  }

  char volumeLabel[MAX_PATH] = {0};
  char fileSystemName[MAX_PATH] = {0};
  DWORD serialNumber = 0;

  if (GetVolumeInformationA(device, volumeLabel, sizeof(volumeLabel), &serialNumber,
                            NULL, NULL, fileSystemName, sizeof(fileSystemName)))
  {
    snprintf(drive.label, sizeof(drive.label), "%s", volumeLabel[0] ? volumeLabel : "Unknown");
    snprintf(drive.fsType, sizeof(drive.fsType), "%s", fileSystemName);
  }

  snprintf(drive.uuid, sizeof(drive.uuid), "%08X", serialNumber);

  char roPath[MAX_PATH];
  snprintf(roPath, sizeof(roPath), "\\\\.\\%s", device);
  HANDLE hDrive = CreateFileA(roPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if (hDrive == INVALID_HANDLE_VALUE)
  {
    drive.readOnly = true;
  }
  else
  {
    drive.readOnly = false;
    CloseHandle(hDrive);
  }

  Logger_Info("[DriveUtils] Retrieved details for %s: Label=%s, UUID=%s, FS=%s, Total=%llu, Used=%llu, Free=%llu, ReadOnly=%s, Removable=%s",
              drive.device, drive.label, drive.uuid, drive.fsType,
              drive.total, drive.used, drive.free,
              drive.readOnly ? "Yes" : "No",
              drive.removable ? "Yes" : "No");

  return drive;
}

bool DriveUtils_MountDrive(const char *device)
{
  (void)device;
  Logger_Info("[DriveUtils] Mounting is not required on Windows.");
  return true;
}

bool DriveUtils_UnmountDrive(const char *device)
{
  Logger_Info("[DriveUtils] Attempting to safely eject drive: %s", device);

  char devicePath[MAX_PATH];
  snprintf(devicePath, sizeof(devicePath), "\\\\.\\%s", device);

  HANDLE hDevice = CreateFileA(devicePath, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

  if (hDevice == INVALID_HANDLE_VALUE)
  {
    Logger_Error("[DriveUtils] Unable to open device for ejecting.");
    return false;
  }

  DWORD bytesReturned;
  BOOL success = DeviceIoControl(hDevice, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0, &bytesReturned, NULL);
  CloseHandle(hDevice);

  if (success)
  {
    Logger_Info("[DriveUtils] Drive %s safely ejected.", device);
    return true;
  }
  else
  {
    Logger_Error("[DriveUtils] Failed to eject drive %s.", device);
    return false;
  }
}
