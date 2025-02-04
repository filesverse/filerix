#include "include/FileSystem/FileUtils.h"
#include "include/FileSystem/DirectoryPermissions.h"
#include "include/Utils/Logger.h"

#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <cstdlib>

namespace fs = std::filesystem;

namespace FileUtils
{
  std::string GetMimeType(const fs::path &filePath)
  {
    try
    {
      if (fs::is_directory(filePath))
      {
        return "directory";
      }

      static const std::unordered_map<std::string, std::string> imageTypes = {
          {".jpg", "image/jpeg"},
          {".jpeg", "image/jpeg"},
          {".png", "image/png"},
          {".gif", "image/gif"},
          {".bmp", "image/bmp"},
          {".tiff", "image/tiff"},
          {".webp", "image/webp"},
          {".svg", "image/svg+xml"}};

      std::string ext = filePath.extension().string();
      auto it = imageTypes.find(ext);
      if (it != imageTypes.end())
      {
        return it->second;
      }
      return "file";
    }
    catch (const std::exception &e)
    {
      Logger::Error("Error getting MIME type for file: " + std::string(e.what()));
      return "unknown";
    }
  }

  std::vector<FileInfo> GetFiles(const std::string &path)
  {
    std::vector<FileInfo> files;

    try
    {
      fs::path fsPath(path);

      if (!fs::exists(fsPath))
      {
        Logger::Error("Error: Directory does not exist: " + path);
        return files;
      }

      if (!DirectoryPermissions::HasAccess(path))
      {
        Logger::Warn("Warning: Insufficient permissions to access the directory: " + path);
        if (!DirectoryPermissions::RequestElevatedPermissions(path))
        {
          Logger::Error("Error: Failed to obtain elevated permissions for directory: " + path);
          return files;
        }
      }

      for (const auto &entry : fs::directory_iterator(fsPath))
      {
        FileInfo fileInfo;
        fileInfo.name = entry.path().filename().string();
        fileInfo.type = GetMimeType(entry.path());
        fileInfo.path = entry.path().string();
        fileInfo.size = fs::is_regular_file(entry.path()) ? fs::file_size(entry.path()) : 0;
        files.push_back(fileInfo);
      }
    }
    catch (const fs::filesystem_error &e)
    {
      Logger::Error("Filesystem error accessing entry in directory " + path + ": " + std::string(e.what()));
    }
    catch (const std::exception &e)
    {
      Logger::Error("Unexpected error in GetFiles: " + std::string(e.what()));
    }

    return files;
  }

  std::vector<FileInfo> SearchFiles(const std::string &path, const std::string &query)
  {
    std::vector<FileInfo> searchResults;
    try
    {
      for (const auto &entry : fs::recursive_directory_iterator(path))
      {
        std::string fileName = entry.path().filename().string();
        if (fileName.find(query) != std::string::npos)
        {
          FileInfo fileInfo;
          fileInfo.name = fileName;
          fileInfo.type = fs::is_directory(entry.path()) ? "directory" : "file";
          fileInfo.path = entry.path().string();
          fileInfo.size = fs::is_regular_file(entry.path()) ? fs::file_size(entry.path()) : 0;
          searchResults.push_back(fileInfo);
        }
      }
    }
    catch (const fs::filesystem_error &e)
    {
      Logger::Error("Filesystem error during search in directory " + path + ": " + std::string(e.what()));
    }
    catch (const std::exception &e)
    {
      Logger::Error("Unexpected error during file search: " + std::string(e.what()));
    }

    return searchResults;
  }

  bool Copy(const std::string &source, const std::string &destination)
  {
    try
    {
      fs::copy(source, destination, fs::copy_options::recursive);
      return true;
    }
    catch (const std::exception &e)
    {
      Logger::Error("Copy failed from " + source + " to " + destination + ": " + std::string(e.what()));
      return false;
    }
  }

  bool Cut(const std::string &source, const std::string &destination)
  {
    try
    {
      if (Copy(source, destination))
      {
        return fs::remove_all(source) > 0;
      }
      return false;
    }
    catch (const std::exception &e)
    {
      Logger::Error("Cut operation failed for " + source + " to " + destination + ": " + std::string(e.what()));
      return false;
    }
  }

  bool Rename(const std::string &oldPath, const std::string &newPath)
  {
    try
    {
      fs::rename(oldPath, newPath);
      return true;
    }
    catch (const std::exception &e)
    {
      Logger::Error("Rename failed from " + oldPath + " to " + newPath + ": " + std::string(e.what()));
      return false;
    }
  }

  bool MoveTo(const std::string &source, const std::string &destination)
  {
    try
    {
      return Rename(source, destination);
    }
    catch (const std::exception &e)
    {
      Logger::Error("Move failed from " + source + " to " + destination + ": " + std::string(e.what()));
      return false;
    }
  }

  bool Compress(const std::string &source, const std::string &destination)
  {
    try
    {
      std::string command = "gzip -c \"" + source + "\" > \"" + destination + "\"";
      int result = std::system(command.c_str());
      if (result != 0)
      {
        Logger::Error("Compression failed for " + source + " to " + destination);
      }
      return result == 0;
    }
    catch (const std::exception &e)
    {
      Logger::Error("Compression error for " + source + " to " + destination + ": " + std::string(e.what()));
      return false;
    }
  }

  bool Decompress(const std::string &source, const std::string &destination)
  {
    try
    {
      std::string command = "gzip -d -c \"" + source + "\" > \"" + destination + "\"";
      int result = std::system(command.c_str());
      if (result != 0)
      {
        Logger::Error("Decompression failed for " + source + " to " + destination);
      }
      return result == 0;
    }
    catch (const std::exception &e)
    {
      Logger::Error("Decompression error for " + source + " to " + destination + ": " + std::string(e.what()));
      return false;
    }
  }
}
