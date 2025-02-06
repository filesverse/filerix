#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <stdint.h>
#include <stdbool.h>
struct FileInfo
{
  char *name;
  char *type;
  char *path;
  uintmax_t size;
  bool isDirectory;
};

#endif
