#include "include/FileSystem/DriveUtils.h"

#include <cstring>
#include <cstdlib>

extern "C"
{
  DriveUsage GetDriveUsage(const char *drive)
  {
    DriveUsage usage = {0, 0};
    auto result = DriveUtils::GetDriveUsage(drive);
    usage.used = result.used;
    usage.total = result.total;
    return usage;
  }

  bool MountDrive(const char *device)
  {
    return DriveUtils::MountDrive(device);
  }

  bool UnmountDrive(const char *device)
  {
    return DriveUtils::UnmountDrive(device);
  }

  const char *GetDeviceLabelOrUUID(const char *device)
  {
    try
    {
      std::string label = DriveUtils::GetDeviceLabelOrUUID(device);
      return strdup(label.c_str());
    }
    catch (...)
    {
      return NULL;
    }
  }

  DriveList GetDrives()
  {
    auto cppDrives = DriveUtils::GetDrives();
    DriveInfo *drives = (DriveInfo *)malloc(cppDrives.size() * sizeof(DriveInfo));

    for (size_t i = 0; i < cppDrives.size(); ++i)
    {
      strcpy(drives[i].device, cppDrives[i].device.c_str());
      strcpy(drives[i].status, cppDrives[i].status.c_str());
      strcpy(drives[i].mountPoint, cppDrives[i].mountPoint.c_str());
      strcpy(drives[i].partition, cppDrives[i].partition.c_str());
      strcpy(drives[i].fsType, cppDrives[i].fsType.c_str());
      drives[i].unmountable = cppDrives[i].unmountable;
    }

    return (DriveList){.items = drives, .count = cppDrives.size()};
  }
}
