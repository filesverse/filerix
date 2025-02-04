#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "FileInfo.h"

#ifdef __cplusplus
#include <string>
#include <vector>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  bool Copy(const char *source, const char *destination);
  bool Cut(const char *source, const char *destination);
  bool Rename(const char *oldPath, const char *newPath);
  bool MoveTo(const char *source, const char *destination);
  bool Compress(const char *source, const char *destination);
  bool Decompress(const char *source, const char *destination);

  struct FileInfo *GetFiles(const char *path, size_t *count);
  struct FileInfo *SearchFiles(const char *path, const char *query, size_t *count);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
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

#endif
