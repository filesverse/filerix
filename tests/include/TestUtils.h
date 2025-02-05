#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

  bool CreateFile(const char *path);
  bool Cleanup(const char *path);
  int FileExists(const char *path);
  void setup_redirect();

#ifdef __cplusplus
}
#endif

#endif
