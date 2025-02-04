#include "include/FileSystem/DirectoryPermissions.h"
#include "include/Utils/Logger.h"

#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

namespace fs = std::filesystem;

namespace DirectoryPermissions
{
  bool HasAccess(const std::string &path)
  {
    try
    {
      fs::path fsPath(path);

      if (fs::exists(fsPath))
      {
        Logger::Info("Checking access for: " + path);
        if (fs::is_directory(fsPath))
        {
          Logger::Info("Path is a directory.");
        }
        else
        {
          Logger::Warn("Path exists but is not a directory: " + path);
        }

        if (access(path.c_str(), R_OK) == 0)
        {
          Logger::Info("Access granted to directory: " + path);
          return true;
        }
        else
        {
          Logger::Error("Access denied to directory: " + path);
          return false;
        }
      }
      else
      {
        Logger::Warn("Directory does not exist: " + path);
        return false;
      }
    }
    catch (const std::exception &e)
    {
      Logger::Error("Exception occurred while checking access for " + path + ": " + e.what());
      return false;
    }
  }

  bool RequestElevatedPermissions(const std::string &path)
  {
    try
    {
      Logger::Info("Requesting elevated permissions for: " + path);

      std::string command = "pkexec ls \"" + path + "\"";
      int result = std::system(command.c_str());

      if (result == 0)
      {
        Logger::Info("Elevated permissions granted for: " + path);
        return true;
      }
      else
      {
        Logger::Error("Failed to request elevated permissions for: " + path);
        return false;
      }
    }
    catch (const std::exception &e)
    {
      Logger::Error("Exception occurred while requesting elevated permissions for " + path + ": " + e.what());
      return false;
    }
  }
}
