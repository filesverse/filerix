#include "System/UserUtils.h"
#include <string>
#include <cstdlib>

namespace UserUtils
{
  std::string GetUserName()
  {
    const char *name = UserUtils_GetUserName();
    if (name)
    {
      std::string result(name);
      free((void *)name);
      return result;
    }
    return "";
  }

  bool ChangePermissions(const std::string &path, int mode)
  {
    return UserUtils_ChangePermissions(path.c_str(), mode);
  }
}
