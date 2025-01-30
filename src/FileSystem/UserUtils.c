#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include "utils/Logger.h"

char *GetUserName()
{
  struct passwd *pw = getpwuid(getuid());
  if (pw)
  {
    char *username = (char *)malloc(strlen(pw->pw_name) + 1);
    if (username)
    {
      strcpy(username, pw->pw_name);
      return username;
    }
    else
    {
      logger_error("Failed to allocate memory for username");
      return NULL;
    }
  }
  else
  {
    logger_error("Failed to get username: getpwuid returned null");
    return NULL;
  }
}

int ChangePermissions(const char *path, mode_t mode)
{
  if (chmod(path, mode) != 0)
  {
    logger_error("Failed to change permissions for %s", path);
    return 0;
  }
  return 1;
}
