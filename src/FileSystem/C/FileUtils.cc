#include "FileSystem/FileUtils.h"
#include "Utils/Logger.h"

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

  void CopyStringToCharArray(const std::string &src, char *dest, size_t maxLen)
  {
    strncpy(dest, src.c_str(), maxLen - 1);
    dest[maxLen - 1] = '\0';
  }

  FileList GetFiles(const char *path)
  {
    auto files = FileUtils::GetFiles(std::string(path));
    struct FileList result;
    result.count = files.size();
    result.files = (struct FileInfo *)malloc(result.count * sizeof(struct FileInfo));

    if (result.files == NULL)
    {
      Logger::Error("[FileUtils] Memory allocation failed for FileInfo array");
    }

    for (size_t i = 0; i < result.count; ++i)
    {
      result.files[i].name = (char *)malloc(256 * sizeof(char));
      result.files[i].type = (char *)malloc(256 * sizeof(char));
      result.files[i].path = (char *)malloc(256 * sizeof(char));

      CopyStringToCharArray(files[i].name, result.files[i].name, 256);
      CopyStringToCharArray(files[i].type, result.files[i].type, 256);
      CopyStringToCharArray(files[i].path, result.files[i].path, 256);
      result.files[i].size = files[i].size;
      result.files[i].isDirectory = files[i].isDirectory;
    }

    return result;
  }

  FileList SearchFiles(const char *path, const char *query)
  {
    auto files = FileUtils::SearchFiles(std::string(path), std::string(query));
    struct FileList result;
    result.count = files.size();
    result.files = (struct FileInfo *)malloc(result.count * sizeof(struct FileInfo));

    if (result.files == NULL)
    {
      Logger::Error("[FileUtils] Memory allocation failed for FileInfo array");
    }

    for (size_t i = 0; i < result.count; ++i)
    {
      result.files[i].name = (char *)malloc(256 * sizeof(char));
      result.files[i].type = (char *)malloc(256 * sizeof(char));
      result.files[i].path = (char *)malloc(256 * sizeof(char));

      CopyStringToCharArray(files[i].name, result.files[i].name, 256);
      CopyStringToCharArray(files[i].type, result.files[i].type, 256);
      CopyStringToCharArray(files[i].path, result.files[i].path, 256);
      result.files[i].size = files[i].size;
      result.files[i].isDirectory = files[i].isDirectory;
    }

    return result;
  }
}
