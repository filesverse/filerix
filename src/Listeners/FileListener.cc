#include "Listeners/FileListener.h"
#include <iostream>
#include <functional>
#include <string>
#include <atomic>

namespace FileListener
{
  static std::atomic<bool> running = false;
  static std::function<void(std::string, std::string)> eventCallback;

  void FileListener_Callback(const char *eventType, const char *filePath)
  {
    if (eventCallback)
    {
      eventCallback(std::string(eventType), std::string(filePath));
    }
  }

  void Start(const std::string &directory, std::function<void(std::string, std::string)> callback)
  {
    if (running.load())
      return;

    running.store(true);
    eventCallback = callback;

    FileListener_Start(directory.c_str(), FileListener_Callback);
  }

  void Stop()
  {
    if (!running.load())
      return;

    running.store(false);
    FileListener_Stop();
  }
}
