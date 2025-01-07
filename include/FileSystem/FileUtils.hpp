#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include "FileInfo.hpp"

namespace FileUtils
{
  std::vector<FileInfo> GetFiles(const std::string &path);
  std::vector<FileInfo> SearchFiles(const std::string &path, const std::string &query);
}

#endif
