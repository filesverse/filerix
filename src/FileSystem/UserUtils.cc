#include "FileSystem/UserUtils.h"
#include "Utils/Logger.h"

#include <string>
#include <stdexcept>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

namespace UserUtils
{
  std::string GetUserName()
  {
    try
    {
      struct passwd *pw = getpwuid(getuid());
      if (pw)
      {
        return std::string(pw->pw_name);
      }
      else
      {
        Logger::Error("[UserUtils] Failed to get username: getpwuid returned null");
      }
    }
    catch (const std::exception &e)
    {
      Logger::Error("[UserUtils] Error retrieving username: " + std::string(e.what()));
    }
  }

  bool ChangePermissions(const std::string &path, int mode)
  {
    try
    {
      if (chmod(path.c_str(), mode) != 0)
      {
        Logger::Error("[UserUtils] Failed to change permissions for " + path);
        return false;
      }
      return true;
    }
    catch (const std::exception &e)
    {
      Logger::Error("[UserUtils] Error changing permissions for " + path + ": " + std::string(e.what()));
      return false;
    }
  }
}
