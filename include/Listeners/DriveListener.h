#ifndef DRIVE_LISTENER_H
#define DRIVE_LISTENER_H

#ifdef __cplusplus
extern "C"
{
#endif

  typedef void (*DriveEventCallback)(const char *action, const char *device);

  void DriveListener_Start(DriveEventCallback callback);
  void DriveListener_Stop();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <thread>
#include <atomic>
#include <functional>

namespace DriveListener
{
  void Start(std::function<void(std::string, std::string)> callback);
  void Stop();
}

#endif

#endif
