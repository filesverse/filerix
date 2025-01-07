#ifndef DIRECTORY_PERMISSIONS_H
#define DIRECTORY_PERMISSIONS_H

#include <string>

namespace DirectoryPermissions
{
  bool HasAccess(const std::string &path);
  bool RequestElevatedPermissions(const std::string &path);
}

#endif
