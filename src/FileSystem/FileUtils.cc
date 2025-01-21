#include "include/FileSystem/FileUtils.hpp"
#include "include/FileSystem/DirectoryPermissions.hpp"
#include "include/utils/Logger.hpp"
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
    if (imageTypes.find(ext) != imageTypes.end())
    {
      return imageTypes.at(ext);
    }
    return "file";
  }

  std::vector<FileInfo> GetFiles(const std::string &path)
  {
    std::vector<FileInfo> files;
    try
    {
      fs::path fsPath(path);
      if (!fs::exists(fsPath))
      {
        std::cerr << "Error: Directory does not exist: " << path << "\n";
        return {};
      }

      if (!DirectoryPermissions::HasAccess(path))
      {
        std::cerr << "Warning: Insufficient permissions to access the directory: " << path << "\n";
        if (!DirectoryPermissions::RequestElevatedPermissions(path))
        {
          std::cerr << "Error: Failed to obtain elevated permissions for directory: " << path << "\n";
          return {};
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
      std::cerr << "Filesystem error accessing entry: " << e.what() << "\n";
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
      throw std::runtime_error(e.what());
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
      Logger::Error("Copy failed: " + std::string(e.what()));
      return false;
    }
  }

  bool Cut(const std::string &source, const std::string &destination)
  {
    if (Copy(source, destination))
    {
      return fs::remove_all(source) > 0;
    }
    return false;
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
      Logger::Error("Rename failed: " + std::string(e.what()));
      return false;
    }
  }

  bool MoveTo(const std::string &source, const std::string &destination)
  {
    return Rename(source, destination);
  }

  bool Compress(const std::string &source, const std::string &destination)
  {
    std::string command = "gzip -c \"" + source + "\" > \"" + destination + "\"";
    return std::system(command.c_str()) == 0;
  }

  bool Decompress(const std::string &source, const std::string &destination)
  {
    std::string command = "gzip -d -c \"" + source + "\" > \"" + destination + "\"";
    return std::system(command.c_str()) == 0;
  }
}
