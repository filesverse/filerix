#include "Listeners/DriveListener.h"
#include <iostream>
#include <functional>
#include <string>
#include <atomic>

namespace DriveListener
{
  static std::atomic<bool> running = false;
  static std::function<void(std::string, std::string)> eventCallback;

  void DriveListener_Callback(const char *action, const char *device)
  {
    if (eventCallback)
    {
      eventCallback(std::string(action), std::string(device));
    }
  }

  void Start(std::function<void(std::string, std::string)> callback)
  {
    if (running.load())
      return;

    running.store(true);
    eventCallback = callback;

    DriveListener_Start(DriveListener_Callback);
  }

  void Stop()
  {
    if (!running.load())
      return;

    running.store(false);
    DriveListener_Stop();
  }
}
