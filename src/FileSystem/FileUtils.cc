#include "include/FileSystem/FileUtils.hpp"
#include "include/FileSystem/DirectoryPermissions.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;

namespace FileUtils
{
  std::vector<FileInfo> GetFiles(const std::string &path)
  {
    std::vector<FileInfo> files;
    try
    {
      fs::path fsPath(path);
      if (!fs::exists(fsPath))
      {
        throw std::runtime_error("Directory does not exist.");
      }

      if (!DirectoryPermissions::HasAccess(path))
      {
        std::cout << "Insufficient permissions to access this directory.\nAttempting to request elevated permissions...\n";
        if (!DirectoryPermissions::RequestElevatedPermissions(path))
        {
          throw std::runtime_error("Failed to obtain elevated permissions.");
        }
      }

      if (fs::exists(fsPath) && fs::is_directory(fsPath))
      {
        for (const auto &entry : fs::directory_iterator(fsPath))
        {
          FileInfo fileInfo;
          fileInfo.name = entry.path().filename().string();
          fileInfo.type = fs::is_directory(entry.path()) ? "directory" : "file";
          fileInfo.size = fs::is_regular_file(entry.path()) ? fs::file_size(entry.path()) : 0;
          files.push_back(fileInfo);
        }
      }
    }
    catch (const fs::filesystem_error &e)
    {
      throw std::runtime_error(e.what());
    }
    return files;
  }

  std::vector<FileInfo> SearchFiles(const std::string &path, const std::string &query)
  {
    std::vector<FileInfo> searchResults;
    try
    {
      fs::path fsPath(path);
      if (!fs::exists(fsPath) || !fs::is_directory(fsPath))
      {
        throw std::runtime_error("Directory does not exist or is not accessible.");
      }

      for (const auto &entry : fs::directory_iterator(fsPath))
      {
        std::string fileName = entry.path().filename().string();

        if (fileName.find(query) != std::string::npos)
        {
          FileInfo fileInfo;
          fileInfo.name = fileName;
          fileInfo.type = fs::is_directory(entry.path()) ? "directory" : "file";
          fileInfo.size = fs::is_regular_file(entry.path()) ? fs::file_size(entry.path()) : 0;
          searchResults.push_back(fileInfo);
        }

        if (fs::is_directory(entry.path()))
        {
          for (const auto &subEntry : fs::directory_iterator(entry.path()))
          {
            std::string subFileName = subEntry.path().filename().string();
            if (subFileName.find(query) != std::string::npos)
            {
              FileInfo subFileInfo;
              subFileInfo.name = subFileName;
              subFileInfo.type = fs::is_directory(subEntry.path()) ? "directory" : "file";
              subFileInfo.size = fs::is_regular_file(subEntry.path()) ? fs::file_size(subEntry.path()) : 0;
              searchResults.push_back(subFileInfo);
            }
          }
        }
      }
    }
    catch (const fs::filesystem_error &e)
    {
      throw std::runtime_error(e.what());
    }
    return searchResults;
  }
}
