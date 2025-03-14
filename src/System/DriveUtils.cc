#include "System/DriveUtils.h"
#include <string>
#include <vector>

namespace DriveUtils
{
  std::string GetMountPoint(const std::string &device)
  {
    const char *mountPoint = DriveUtils_GetMountPoint(device.c_str());
    if (mountPoint)
    {
      std::string result(mountPoint);
      free((void *)mountPoint);
      return result;
    }
    return "";
  }

  std::vector<DriveUtils::DriveInfo> GetDrives()
  {
    DriveList driveList = DriveUtils_GetDrives();
    std::vector<DriveUtils::DriveInfo> result;
    result.reserve(driveList.count);

    for (size_t i = 0; i < driveList.count; ++i)
    {
      ::DriveInfo cDrive = driveList.drives[i];
      result.push_back(DriveUtils::DriveInfo{
          std::string(cDrive.device),
          std::string(cDrive.status),
          cDrive.unmountable,
          std::string(cDrive.mountPoint),
          std::string(cDrive.partition),
          std::string(cDrive.fsType)});
    }

    free(driveList.drives);
    return result;
  }

  DriveUsage GetDriveUsage(const std::string &drive)
  {
    DriveUsage cUsage = DriveUtils_GetDriveUsage(drive.c_str());
    return DriveUtils::DriveUsage{cUsage.used, cUsage.total};
  }

  std::string GetDeviceLabelOrUUID(const std::string &pathOrDevice)
  {
    const char *labelOrUUID = DriveUtils_GetDeviceLabelOrUUID(pathOrDevice.c_str());
    if (labelOrUUID)
    {
      std::string result(labelOrUUID);
      free((void *)labelOrUUID);
      return result;
    }
    return "";
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
