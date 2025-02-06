#include "include/FileSystem/FileUtils.h"
#include "include/FileSystem/DirectoryPermissions.h"
#include "include/Utils/Logger.h"

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
        fileInfo.name = CopyString(entry.path().filename().string());
        fileInfo.type = CopyString(GetMimeType(entry.path()));
        fileInfo.path = CopyString(entry.path().string());
        fileInfo.size = fs::is_regular_file(entry.path()) ? fs::file_size(entry.path()) : 0;
        fileInfo.isDirectory = fs::is_directory(entry.path());

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
          fileInfo.name = CopyString(entry.path().filename().string());
          fileInfo.type = CopyString(GetMimeType(entry.path()));
          fileInfo.path = CopyString(entry.path().string());
          fileInfo.size = fs::is_regular_file(entry.path()) ? fs::file_size(entry.path()) : 0;
          fileInfo.isDirectory = fs::is_directory(entry.path());

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
    std::ifstream inFile(source, std::ios::binary);
    if (!inFile)
    {
      Logger::Error("Failed to open source file: " + source);
      return false;
    }

    std::ofstream outFile(destination, std::ios::binary);
    if (!outFile)
    {
      Logger::Error("Failed to create destination file: " + destination);
      return false;
    }

    unsigned char header[10] = {0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00}; // GZIP header
    outFile.write(reinterpret_cast<char *>(header), sizeof(header));

    std::vector<char> buffer(1024);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    if (deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
      Logger::Error("Failed to initialize zlib for compression");
      return false;
    }

    while (inFile.read(buffer.data(), buffer.size()) || inFile.gcount() > 0)
    {
      strm.avail_in = inFile.gcount();
      strm.next_in = reinterpret_cast<unsigned char *>(buffer.data());

      do
      {
        std::vector<char> outBuffer(1024);
        strm.avail_out = outBuffer.size();
        strm.next_out = reinterpret_cast<unsigned char *>(outBuffer.data());

        if (deflate(&strm, Z_NO_FLUSH) == Z_STREAM_ERROR)
        {
          Logger::Error("Compression error");
          return false;
        }

        outFile.write(outBuffer.data(), outBuffer.size() - strm.avail_out);
      } while (strm.avail_out == 0);
    }

    do
    {
      std::vector<char> outBuffer(1024);
      strm.avail_out = outBuffer.size();
      strm.next_out = reinterpret_cast<unsigned char *>(outBuffer.data());

      if (deflate(&strm, Z_FINISH) == Z_STREAM_ERROR)
      {
        Logger::Error("Compression error");
        return false;
      }

      outFile.write(outBuffer.data(), outBuffer.size() - strm.avail_out);
    } while (strm.avail_out == 0);

    deflateEnd(&strm);

    return true;
  }

  bool Decompress(const std::string &source, const std::string &destination)
  {
    std::ifstream inFile(source, std::ios::binary);
    if (!inFile)
    {
      Logger::Error("Failed to open compressed file: " + source);
      return false;
    }

    std::ofstream outFile(destination, std::ios::binary);
    if (!outFile)
    {
      Logger::Error("Failed to create destination file: " + destination);
      return false;
    }

    // Skip the gzip header (10 bytes)
    inFile.seekg(10, std::ios::beg);

    std::vector<char> buffer(1024);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    if (inflateInit2(&strm, MAX_WBITS + 16) != Z_OK)
    {
      Logger::Error("Failed to initialize zlib for decompression");
      return false;
    }

    while (inFile.read(buffer.data(), buffer.size()) || inFile.gcount() > 0)
    {
      strm.avail_in = inFile.gcount();
      strm.next_in = reinterpret_cast<unsigned char *>(buffer.data());

      do
      {
        std::vector<char> outBuffer(1024);
        strm.avail_out = outBuffer.size();
        strm.next_out = reinterpret_cast<unsigned char *>(outBuffer.data());

        if (inflate(&strm, Z_NO_FLUSH) == Z_STREAM_ERROR)
        {
          Logger::Error("Decompression error");
          return false;
        }

        outFile.write(outBuffer.data(), outBuffer.size() - strm.avail_out);
      } while (strm.avail_out == 0);
    }

    do
    {
      std::vector<char> outBuffer(1024);
      strm.avail_out = outBuffer.size();
      strm.next_out = reinterpret_cast<unsigned char *>(outBuffer.data());

      if (inflate(&strm, Z_FINISH) == Z_STREAM_ERROR)
      {
        Logger::Error("Decompression error");
        return false;
      }

      outFile.write(outBuffer.data(), outBuffer.size() - strm.avail_out);
    } while (strm.avail_out == 0);

    inflateEnd(&strm);

    return true;
  }
}
