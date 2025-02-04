#ifndef DRIVEUTILS_H
#define DRIVEUTILS_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    unsigned long long used;
    unsigned long long total;
  } DriveUsage;

  typedef struct
  {
    char device[256];
    char status[256];
    char mountPoint[256];
    char partition[256];
    char fsType[256];
    bool unmountable;
  } DriveInfo;

  typedef struct
  {
    DriveInfo *items;
    size_t count;
  } DriveList;

  DriveUsage GetDriveUsage(const char *drive);
  const char *GetMountPoint(const char *device);
  bool MountDrive(const char *device);
  bool UnmountDrive(const char *device);
  const char *GetDeviceLabelOrUUID(const char *device);
  DriveList GetDrives();

#ifdef __cplusplus
}
#endif

#endif

#ifdef __cplusplus

#include <string>
#include <vector>
#include <utility>

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

  std::vector<DriveInfo> GetDrives();
  DriveUsage GetDriveUsage(const std::string &drive);
  std::string GetDeviceLabelOrUUID(const std::string &device);
  bool MountDrive(const std::string &device);
  bool UnmountDrive(const std::string &device);
}

#endif
