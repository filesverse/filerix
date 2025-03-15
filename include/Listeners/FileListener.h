#ifndef FILE_LISTENER_H
#define FILE_LISTENER_H

#ifdef __cplusplus
extern "C"
{
#endif

  typedef void (*FileEventCallback)(const char *eventType, const char *filePath);

  void FileListener_Start(const char *directory, FileEventCallback callback);
  void FileListener_Stop();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <string>
#include <functional>
#include <atomic>
#include <thread>

namespace FileListener
{
  void Start(const std::string &directory, std::function<void(std::string, std::string)> callback);
  void Stop();
}

#endif

#endif
