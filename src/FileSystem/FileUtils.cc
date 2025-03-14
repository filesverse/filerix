#include "FileSystem/FileUtils.h"
#include <cstdlib>
#include <vector>

namespace FileUtils
{
  std::vector<FileInfo> GetFiles(const std::string &path)
  {
    struct FileList fileList = FileUtils_GetFiles(path.c_str());

    std::vector<FileInfo> files;
    if (!fileList.files)
      return files;

    for (size_t i = 0; i < fileList.count; i++)
    {
      char *name = fileList.files[i].name;
      char *type = fileList.files[i].type;
      char *path = fileList.files[i].path;

      files.push_back(FileInfo{
          name,
          type,
          path,
          fileList.files[i].size,
          fileList.files[i].isDirectory});

      free(name);
      free(type);
      free(path);
    }

    free(fileList.files);
    return files;
  }

  bool Copy(const std::string &source, const std::string &destination)
  {
    return FileUtils_Copy(source.c_str(), destination.c_str());
  }

  bool Rename(const std::string &oldPath, const std::string &newPath)
  {
    return FileUtils_Rename(oldPath.c_str(), newPath.c_str());
  }

  bool MoveTo(const std::string &source, const std::string &destination)
  {
    return FileUtils_MoveTo(source.c_str(), destination.c_str());
  }

  bool Compress(const std::string &source, const std::string &destination)
  {
    return FileUtils_Compress(source.c_str(), destination.c_str());
  }

  bool Decompress(const std::string &source, const std::string &destination)
  {
    return FileUtils_Decompress(source.c_str(), destination.c_str());
  }
}
