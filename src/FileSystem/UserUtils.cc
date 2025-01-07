#include <string>
#include <stdexcept>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

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
}
