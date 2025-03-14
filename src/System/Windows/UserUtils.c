#include "System/UserUtils.h"
#include "Utils/Logger.h"

#include <windows.h>
#include <lmcons.h>
#include <aclapi.h>
#include <io.h>

const char *UserUtils_GetUserName()
{
  char username[UNLEN + 1];
  DWORD username_len = UNLEN + 1;

  if (GetUserNameA(username, &username_len))
  {
    return _strdup(username);
  }
  else
  {
    Logger_Error("[UserUtils] Failed to get username: GetUserNameA returned error");
    return NULL;
  }
}

bool UserUtils_ChangePermissions(const char *path, int mode)
{
  DWORD attributes = GetFileAttributesA(path);
  if (attributes == INVALID_FILE_ATTRIBUTES)
  {
    Logger_Error("[UserUtils] Failed to get file attributes");
    return false;
  }

  if (mode & 0400)
    attributes |= FILE_ATTRIBUTE_READONLY;
  else
    attributes &= ~FILE_ATTRIBUTE_READONLY;

  if (!SetFileAttributesA(path, attributes))
  {
    Logger_Error("[UserUtils] Failed to change file attributes");
    return false;
  }

  return true;
}
