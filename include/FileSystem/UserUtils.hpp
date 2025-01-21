#ifndef USERUTILS_H
#define USERUTILS_H

#include <string>

namespace UserUtils
{
  std::string GetUserName();
  bool ChangePermissions(const std::string &path, int mode);
}

#endif
