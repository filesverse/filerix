#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include "FileInfo.hpp"

namespace FileUtils
{
  std::vector<FileInfo> GetFiles(const std::string &path);
  std::vector<FileInfo> SearchFiles(const std::string &path, const std::string &query);
  bool Copy(const std::string &source, const std::string &destination);
  bool Cut(const std::string &source, const std::string &destination);
  bool Rename(const std::string &oldPath, const std::string &newPath);
  bool MoveTo(const std::string &source, const std::string &destination);
  bool Compress(const std::string &source, const std::string &destination);
  bool Decompress(const std::string &source, const std::string &destination);
}

#endif
