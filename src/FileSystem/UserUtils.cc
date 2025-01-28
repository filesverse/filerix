#include <string>
#include <stdexcept>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include "include/utils/Logger.hpp"

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
        Logger::Error("Failed to get username: getpwuid returned null");
        throw std::runtime_error("Failed to get username");
      }
    }
    catch (const std::exception &e)
    {
      Logger::Error("Error retrieving username: " + std::string(e.what()));
      throw;
    }
  }

  bool ChangePermissions(const std::string &path, int mode)
  {
    try
    {
      if (chmod(path.c_str(), mode) != 0)
      {
        Logger::Error("Failed to change permissions for " + path);
        return false;
      }
      return true;
    }
    catch (const std::exception &e)
    {
      Logger::Error("Error changing permissions for " + path + ": " + std::string(e.what()));
      return false;
    }
  }
}
