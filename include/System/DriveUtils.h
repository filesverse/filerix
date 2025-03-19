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
    bool readOnly;
    bool removable;
    char device[256];
    char status[256];
    char mountPoint[256];
    char fsType[256];
    char label[256];
    char uuid[256];
    uint64_t total;
    uint64_t used;
    uint64_t free;
  } DriveInfo;

  typedef struct DriveList
  {
    char **devices;
    size_t count;
  } DriveList;

  DriveList DriveUtils_GetDrives();
  DriveInfo DriveUtils_GetDriveInfo(const char *device);
  bool DriveUtils_MountDrive(const char *device);
  bool DriveUtils_UnmountDrive(const char *device);

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
    bool readOnly;
    bool removable;
    std::string device;
    std::string status;
    std::string mountPoint;
    std::string fsType;
    std::string label;
    std::string uuid;
    uint64_t total;
    uint64_t used;
    uint64_t free;
  };

  std::vector<std::string> GetDrives();
  DriveInfo GetDriveInfo(const std::string &device);
  bool MountDrive(const std::string &device);
  bool UnmountDrive(const std::string &device);
}

#endif

#endif