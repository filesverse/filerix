#ifndef FILE_LISTENER_H
#define FILE_LISTENER_H

#ifdef __cplusplus
extern "C"
{
#endif

  typedef void (*FileEventCallback)(const char *eventType, const char *filePath);

  void StartFileMonitor(const char *directory, FileEventCallback callback);
  void StopFileMonitor();

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
  class FileMonitor
  {
  public:
    explicit FileMonitor(const std::string &directory);
    ~FileMonitor();

    void Start(std::function<void(const std::string &, const std::string &)> callback);
    void Stop();

  private:
    std::string watchedDirectory;
    std::atomic<bool> running;
    std::thread monitorThread;
    std::function<void(const std::string &, const std::string &)> eventCallback;
  };
}

#endif

#endif
