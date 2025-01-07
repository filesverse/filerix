#include "include/FileSystem/DirectoryPermissions.hpp"
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

namespace fs = std::filesystem;

namespace DirectoryPermissions
{
  bool HasAccess(const std::string &path)
  {
    fs::path fsPath(path);
    return fs::exists(fsPath) && fs::is_directory(fsPath) && access(path.c_str(), R_OK) == 0;
  }

  bool RequestElevatedPermissions(const std::string &path)
  {
    std::string command = "pkexec " + path;
    int result = std::system(command.c_str());
    return result == 0;
  }
}
