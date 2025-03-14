#ifndef USERUTILS_H
#define USERUTILS_H

#include <sys/types.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <string>

extern "C"
{
#endif

  const char *UserUtils_GetUserName();
  bool UserUtils_ChangePermissions(const char *path, int mode);

#ifdef __cplusplus
}

namespace UserUtils
{
  std::string GetUserName();
  bool ChangePermissions(const std::string &path, int mode);
}

#endif

#endif
