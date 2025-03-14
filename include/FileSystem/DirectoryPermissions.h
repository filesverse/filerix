#ifndef DIRECTORY_PERMISSIONS_H
#define DIRECTORY_PERMISSIONS_H

#include <stdbool.h>

bool MountFuseOverlay(const char *target);
bool UnmountFuse();
bool HasDirectAccess(const char *path);
bool RequestAccessViaDBus(const char *path);
bool HasAccess(const char *path);

#endif
