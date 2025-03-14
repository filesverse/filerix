#ifndef DRIVEUTILS_H
#define DRIVEUTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct DriveUsage
  {
    uint64_t used;
    uint64_t total;
  } DriveUsage;

  typedef struct DriveInfo
  {
    char device[256];
    char status[256];
    char mountPoint[256];
    char partition[256];
    char fsType[256];
    bool unmountable;
  } DriveInfo;

  typedef struct DriveList
  {
    DriveInfo *drives;
    size_t count;
  } DriveList;

  DriveUsage DriveUtils_GetDriveUsage(const char *drive);
  const char *DriveUtils_GetMountPoint(const char *device);
  bool DriveUtils_MountDrive(const char *device);
  bool DriveUtils_UnmountDrive(const char *device);
  const char *DriveUtils_GetDeviceLabelOrUUID(const char *device);
  DriveList DriveUtils_GetDrives();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string>
#include <vector>

namespace DriveUtils
{
  struct DriveInfo
  {
    std::string device;
    std::string status;
    bool unmountable;
    std::string mountPoint;
    std::string partition;
    std::string fsType;
  };

  using DriveUsage = ::DriveUsage;

  std::vector<DriveInfo> GetDrives();
  DriveUsage GetDriveUsage(const std::string &drive);
  std::string GetDeviceLabelOrUUID(const std::string &device);
  bool MountDrive(const std::string &device);
  bool UnmountDrive(const std::string &device);
}

#endif

#endif