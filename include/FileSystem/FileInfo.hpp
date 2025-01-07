#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <string>
#include <cstdint>

struct FileInfo
{
  std::string name;
  std::string type;
  uintmax_t size;
};

#endif
