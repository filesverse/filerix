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
    struct passwd *pw = getpwuid(getuid());
    if (pw)
    {
      return std::string(pw->pw_name);
    }
    else
    {
      throw std::runtime_error("Failed to get username");
    }
  }

  bool ChangePermissions(const std::string &path, int mode)
  {
    if (chmod(path.c_str(), mode) != 0)
    {
      return false;
    }
    return true;
  }
}
