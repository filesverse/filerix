#include "System/DriveUtils.h"
#include <string>
#include <vector>

namespace DriveUtils
{
  std::vector<std::string> GetDrives()
  {
    DriveList driveList = DriveUtils_GetDrives();
    std::vector<std::string> result;
    result.reserve(driveList.count);

    for (size_t i = 0; i < driveList.count; ++i)
    {
      result.emplace_back(driveList.devices[i]);
      free(driveList.devices[i]);
    }

    free(driveList.devices);
    return result;
  }

  DriveInfo GetDriveInfo(const std::string &device)
  {
    ::DriveInfo cDrive = DriveUtils_GetDriveInfo(device.c_str());
    return DriveInfo{
        cDrive.readOnly,
        cDrive.removable,
        std::string(cDrive.device),
        std::string(cDrive.status),
        std::string(cDrive.mountPoint),
        std::string(cDrive.fsType),
        std::string(cDrive.label),
        std::string(cDrive.uuid),
        cDrive.total,
        cDrive.used,
        cDrive.free};
  }

  bool MountDrive(const std::string &device)
  {
    return DriveUtils_MountDrive(device.c_str());
  }

  bool UnmountDrive(const std::string &device)
  {
    return DriveUtils_UnmountDrive(device.c_str());
  }
}
