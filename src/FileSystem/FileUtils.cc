#include "FileSystem/FileUtils.h"
#include "FileSystem/DirectoryPermissions.h"
#include "Utils/Logger.h"
#include "data/FileTypes.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <zlib.h>

namespace fs = std::filesystem;

namespace FileUtils
{
  char *CopyString(const std::string &str)
  {
    char *copy = new char[str.size() + 1];
    std::strcpy(copy, str.c_str());
    return copy;
  }

  std::string GetMimeType(const fs::path &filePath)
  {
    try
    {
      Logger::Info("[FileUtils] Getting MIME type for: " + filePath.string());

      if (fs::is_directory(filePath))
      {
        return "directory";
      }

      std::string ext = filePath.extension().string();

      std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

      auto it = FileTypes::MIME_TYPES.find(ext);
      if (it != FileTypes::MIME_TYPES.end())
      {
        return it->second;
      }

      return "application/octet-stream";
    }
    catch (const std::exception &e)
    {
      Logger::Error("[FileUtils] Error getting MIME type: " + std::string(e.what()));
      return "unknown";
    }
  }

  std::vector<FileInfo> GetFiles(const std::string &path)
  {
    Logger::Info("[FileUtils] Getting files from: " + path);
    std::vector<FileInfo> files;

    try
    {
      fs::path fsPath(path);
      if (!fs::exists(fsPath))
      {
        Logger::Error("[FileUtils] Directory does not exist: " + path);
        return files;
      }

      if (!DirectoryPermissions::HasAccess(path))
      {
        Logger::Warn("[FileUtils] Insufficient permissions: " + path);
        if (!DirectoryPermissions::RequestElevatedPermissions(path))
        {
          Logger::Error("[FileUtils] Failed to get permissions for: " + path);
          return files;
        }
      }

      for (const auto &entry : fs::directory_iterator(fsPath))
      {
        Logger::Info("[FileUtils] Found file: " + entry.path().string());

        FileInfo fileInfo;
        fileInfo.name = CopyString(entry.path().filename().string());
        fileInfo.type = CopyString(GetMimeType(entry.path()));
        fileInfo.path = CopyString(entry.path().string());
        fileInfo.size = fs::is_regular_file(entry.path()) ? fs::file_size(entry.path()) : 0;
        fileInfo.isDirectory = fs::is_directory(entry.path());

        files.push_back(fileInfo);
      }
    }
    catch (const std::exception &e)
    {
      Logger::Error("[FileUtils] Error in GetFiles: " + std::string(e.what()));
    }

    return files;
  }

  std::vector<FileInfo> SearchFiles(const std::string &directory, const std::string &searchQuery)
  {
    Logger::Info("[FileUtils] Searching for files in " + directory + " with query: " + searchQuery);
    std::vector<FileInfo> searchResults;

    try
    {
      fs::path dirPath(directory);
      if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
      {
        Logger::Error("[FileUtils] Directory does not exist or is not accessible: " + directory);
        return searchResults;
      }

      for (const auto &entry : fs::recursive_directory_iterator(dirPath))
      {
        if (entry.path().filename().string().find(searchQuery) != std::string::npos)
        {
          Logger::Info("[FileUtils] Match found: " + entry.path().string());

          FileInfo fileInfo;
          fileInfo.name = CopyString(entry.path().filename().string());
          fileInfo.type = CopyString(GetMimeType(entry.path()));
          fileInfo.path = CopyString(entry.path().string());
          fileInfo.size = fs::is_regular_file(entry.path()) ? fs::file_size(entry.path()) : 0;
          fileInfo.isDirectory = fs::is_directory(entry.path());

          searchResults.push_back(fileInfo);
        }
      }
    }
    catch (const std::exception &e)
    {
      Logger::Error("[FileUtils] Error in SearchFiles: " + std::string(e.what()));
    }

    return searchResults;
  }

  bool Copy(const std::string &source, const std::string &destination)
  {
    Logger::Info("[FileUtils] Copying from " + source + " to " + destination);
    try
    {
      fs::copy(source, destination, fs::copy_options::recursive);
      return true;
    }
    catch (const std::exception &e)
    {
      Logger::Error("[FileUtils] Copy failed: " + std::string(e.what()));
      return false;
    }
  }

  bool Cut(const std::string &source, const std::string &destination)
  {
    Logger::Info("[FileUtils] Cutting from " + source + " to " + destination);
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
      Logger::Error("[FileUtils] Cut failed: " + std::string(e.what()));
      return false;
    }
  }

  bool Rename(const std::string &oldPath, const std::string &newPath)
  {
    Logger::Info("[FileUtils] Renaming " + oldPath + " to " + newPath);
    try
    {
      fs::rename(oldPath, newPath);
      return true;
    }
    catch (const std::exception &e)
    {
      Logger::Error("[FileUtils] Rename failed: " + std::string(e.what()));
      return false;
    }
  }

  bool MoveTo(const std::string &source, const std::string &destination)
  {
    Logger::Info("[FileUtils] Moving from " + source + " to " + destination);
    return Rename(source, destination);
  }

  bool Compress(const std::string &source, const std::string &destination)
  {
    Logger::Info("[FileUtils] Compressing " + source + " to " + destination);
    std::ifstream inFile(source, std::ios::binary);
    if (!inFile)
    {
      Logger::Error("[FileUtils] Failed to open source file: " + source);
      return false;
    }

    std::ofstream outFile(destination, std::ios::binary);
    if (!outFile)
    {
      Logger::Error("[FileUtils] Failed to create destination file: " + destination);
      return false;
    }

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    if (deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
      Logger::Error("[FileUtils] Failed to initialize zlib");
      return false;
    }

    std::vector<char> buffer(1024);
    while (inFile.read(buffer.data(), buffer.size()) || inFile.gcount() > 0)
    {
      strm.avail_in = inFile.gcount();
      strm.next_in = reinterpret_cast<unsigned char *>(buffer.data());

      do
      {
        std::vector<char> outBuffer(1024);
        strm.avail_out = outBuffer.size();
        strm.next_out = reinterpret_cast<unsigned char *>(outBuffer.data());

        deflate(&strm, Z_NO_FLUSH);
        outFile.write(outBuffer.data(), outBuffer.size() - strm.avail_out);
      } while (strm.avail_out == 0);
    }

    deflateEnd(&strm);
    return true;
  }

  bool Decompress(const std::string &source, const std::string &destination)
  {
    Logger::Info("[FileUtils] Decompressing " + source + " to " + destination);
    std::ifstream inFile(source, std::ios::binary);
    if (!inFile)
    {
      Logger::Error("[FileUtils] Failed to open compressed file: " + source);
      return false;
    }

    std::ofstream outFile(destination, std::ios::binary);
    if (!outFile)
    {
      Logger::Error("[FileUtils] Failed to create destination file: " + destination);
      return false;
    }

    inFile.seekg(10, std::ios::beg);

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    if (inflateInit2(&strm, MAX_WBITS + 16) != Z_OK)
    {
      Logger::Error("[FileUtils] Failed to initialize zlib");
      return false;
    }

    std::vector<char> buffer(1024);
    while (inFile.read(buffer.data(), buffer.size()) || inFile.gcount() > 0)
    {
      strm.avail_in = inFile.gcount();
      strm.next_in = reinterpret_cast<unsigned char *>(buffer.data());

      do
      {
        std::vector<char> outBuffer(1024);
        strm.avail_out = outBuffer.size();
        strm.next_out = reinterpret_cast<unsigned char *>(outBuffer.data());

        inflate(&strm, Z_NO_FLUSH);
        outFile.write(outBuffer.data(), outBuffer.size() - strm.avail_out);
      } while (strm.avail_out == 0);
    }

    inflateEnd(&strm);
    return true;
  }
}
