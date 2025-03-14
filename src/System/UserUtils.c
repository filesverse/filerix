#include "System/UserUtils.h"
#include "Utils/Logger.h"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *UserUtils_GetUserName()
{
  struct passwd *pw = getpwuid(getuid());
  if (pw)
  {
    return strdup(pw->pw_name);
  }
  else
  {
    Logger_Error("[UserUtils] Failed to get username: getpwuid returned null");
    return NULL;
  }
}

bool UserUtils_ChangePermissions(const char *path, int mode)
{
  if (chmod(path, mode) != 0)
  {
    Logger_Error("[UserUtils] Failed to change permissions for path");
    return 0;
  }
  return 1;
}
