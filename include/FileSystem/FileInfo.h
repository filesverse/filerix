#ifndef FILE_INFO_H
#define FILE_INFO_H

#ifdef __cplusplus
#include <string>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
  struct FileInfo
  {
    std::string name;
    std::string type;
    std::string path;
    uintmax_t size;
    bool isDirectory;
  };
#else
typedef struct FileInfo
{
  char *name;
  char *type;
  char *path;
  uintmax_t size;
  bool isDirectory;
} FileInfo;
#endif

#ifdef __cplusplus
}
#endif

#endif
