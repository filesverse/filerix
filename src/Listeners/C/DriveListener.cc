#include "include/Listeners/DriveListener.h"

#include <string>
#include <unordered_map>
#include <mutex>

static std::unordered_map<DriveEventCallback, DriveListener::DriveMonitor *> driveMonitors;
static std::mutex driveMonitorMutex;

extern "C"
{

  void StartDriveMonitor(DriveEventCallback callback)
  {
    if (!callback)
      return;

    std::lock_guard<std::mutex> lock(driveMonitorMutex);

    if (driveMonitors.find(callback) != driveMonitors.end())
      return;

    DriveListener::DriveMonitor *monitor = new DriveListener::DriveMonitor();
    driveMonitors[callback] = monitor;

    monitor->Start([callback](const std::string &action, const std::string &device)
                   { callback(action.c_str(), device.c_str()); });
  }

  void StopDriveMonitor()
  {
    std::lock_guard<std::mutex> lock(driveMonitorMutex);

    for (auto &[callback, monitor] : driveMonitors)
    {
      if (monitor)
      {
        monitor->Stop();
        delete monitor;
      }
    }

    driveMonitors.clear();
  }
}