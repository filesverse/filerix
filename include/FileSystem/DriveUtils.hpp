#ifndef DRIVEUTILS_H
#define DRIVEUTILS_H

#include <string>
#include <vector>
#include <utility>

struct DriveInfo
{
  std::string device;
  std::string status;
  bool unmountable;
  std::string mountPoint;
  std::string partition;
};

namespace DriveUtils
{
  std::vector<DriveInfo> GetDrives();
  std::pair<unsigned long long, unsigned long long> GetDriveUsage(const std::string &drive);
  std::string GetDeviceLabelOrUUID(const std::string &device);
  bool MountDrive(const std::string &device);
  bool UnmountDrive(const std::string &device);
}

#endif
