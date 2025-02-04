#include "include/FileSystem/UserUtils.h"

#include <cstring>

extern "C"
{
  const char *GetUserName()
  {
    try
    {
      std::string username = UserUtils::GetUserName();
      return strdup(username.c_str());
    }
    catch (...)
    {
      return nullptr;
    }
  }

  bool ChangePermissions(const char *path, int mode)
  {
    return UserUtils::ChangePermissions(std::string(path), mode);
  }
}