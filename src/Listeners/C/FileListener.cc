#include "include/Listeners/FileListener.h"

#include <string>
#include <unordered_map>
#include <mutex>

static std::unordered_map<FileEventCallback, FileListener::FileMonitor *> fileMonitors;
static std::mutex fileMonitorMutex;

extern "C"
{

  void StartFileMonitor(const char *directory, FileEventCallback callback)
  {
    if (!directory || !callback)
      return;

    std::lock_guard<std::mutex> lock(fileMonitorMutex);

    if (fileMonitors.find(callback) != fileMonitors.end())
      return;

    FileListener::FileMonitor *monitor = new FileListener::FileMonitor(directory);
    fileMonitors[callback] = monitor;

    monitor->Start([callback](const std::string &event, const std::string &filePath)
                   { callback(event.c_str(), filePath.c_str()); });
  }

  void StopFileMonitor()
  {
    std::lock_guard<std::mutex> lock(fileMonitorMutex);

    for (auto &[callback, monitor] : fileMonitors)
    {
      if (monitor)
      {
        monitor->Stop();
        delete monitor;
      }
    }

    fileMonitors.clear();
  }
}
