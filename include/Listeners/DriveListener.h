#ifndef DRIVE_LISTENER_H
#define DRIVE_LISTENER_H

#ifdef __cplusplus
extern "C"
{
#endif

  typedef void (*DriveEventCallback)(const char *action, const char *device);

  void StartDriveMonitor(DriveEventCallback callback);
  void StopDriveMonitor();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <thread>
#include <atomic>
#include <functional>
#include <libudev.h>

namespace DriveListener
{
  class DriveMonitor
  {
  public:
    DriveMonitor();
    ~DriveMonitor();
    void Start(std::function<void(std::string, std::string)> callback);
    void Stop();

  private:
    std::thread monitorThread;
    std::atomic<bool> running;
    std::function<void(std::string, std::string)> eventCallback;
  };
}

#endif

#endif
