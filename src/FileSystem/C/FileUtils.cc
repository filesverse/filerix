#include "include/FileSystem/FileUtils.h"
#include "include/Utils/Logger.h"

#include <cstring>
#include <cstdlib>

extern "C"
{

  bool Copy(const char *source, const char *destination)
  {
    return FileUtils::Copy(std::string(source), std::string(destination));
  }

  bool Cut(const char *source, const char *destination)
  {
    return FileUtils::Cut(std::string(source), std::string(destination));
  }

  bool Rename(const char *oldPath, const char *newPath)
  {
    return FileUtils::Rename(std::string(oldPath), std::string(newPath));
  }

  bool MoveTo(const char *source, const char *destination)
  {
    return FileUtils::MoveTo(std::string(source), std::string(destination));
  }

  bool Compress(const char *source, const char *destination)
  {
    return FileUtils::Compress(std::string(source), std::string(destination));
  }

  bool Decompress(const char *source, const char *destination)
  {
    return FileUtils::Decompress(std::string(source), std::string(destination));
  }

  struct FileInfo *GetFiles(const char *path, size_t *count)
  {
    auto files = FileUtils::GetFiles(std::string(path));
    *count = files.size();

    struct FileInfo *result = (struct FileInfo *)malloc(*count * sizeof(struct FileInfo));
    if (result == NULL)
    {
      Logger::Error("Memory allocation failed for FileInfo array");
      return NULL;
    }

    for (size_t i = 0; i < *count; ++i)
    {
      result[i].name = strdup(files[i].name.c_str());
      result[i].type = strdup(files[i].type.c_str());
      result[i].path = strdup(files[i].path.c_str());
      result[i].size = files[i].size;
      result[i].isDirectory = files[i].isDirectory;
    }

    return result;
  }

  struct FileInfo *SearchFiles(const char *path, const char *query, size_t *count)
  {
    auto files = FileUtils::SearchFiles(std::string(path), std::string(query));
    *count = files.size();

    struct FileInfo *result = (struct FileInfo *)malloc(*count * sizeof(struct FileInfo));

    for (size_t i = 0; i < *count; ++i)
    {
      result[i].name = strdup(files[i].name.c_str());
      result[i].type = strdup(files[i].type.c_str());
      result[i].path = strdup(files[i].path.c_str());
      result[i].size = files[i].size;
      result[i].isDirectory = files[i].isDirectory;
    }

    return result;
  }
}
