#ifndef DRIVEUTILS_H
#define DRIVEUTILS_H

#include <stdbool.h>
#include <stddef.h>

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

#endif
