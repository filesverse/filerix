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
  int RemoveDir(const char *path);
  int CreateDir(const char *path);
  void SetupRedirect();

#ifdef __cplusplus
}
#endif

#endif
