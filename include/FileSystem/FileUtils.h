#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "FileInfo.h"
#include <stddef.h>

#ifdef __cplusplus
#include <string>
#include <vector>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  bool FileUtils_Copy(const char *source, const char *destination);
  bool FileUtils_Cut(const char *source, const char *destination);
  bool FileUtils_Rename(const char *oldPath, const char *newPath);
  bool FileUtils_MoveTo(const char *source, const char *destination);
  bool FileUtils_Compress(const char *source, const char *destination);
  bool FileUtils_Decompress(const char *source, const char *destination);

  struct FileList
  {
    struct FileInfo *files;
    size_t count;
  };

  struct FileList FileUtils_GetFiles(const char *path);
  struct FileList FileUtils_SearchFiles(const char *path, const char *query);

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
