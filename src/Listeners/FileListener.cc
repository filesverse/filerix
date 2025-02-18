#include "Listeners/FileListener.h"
#include "Utils/Logger.h"

#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

FileListener::FileMonitor::FileMonitor(const std::string &directory) : watchedDirectory(directory), running(false) {}

FileListener::FileMonitor::~FileMonitor()
{
  Stop();
}

void FileListener::FileMonitor::Start(std::function<void(const std::string &, const std::string &)> callback)
{
  if (running.load())
    return;

  running.store(true);
  eventCallback = callback;

  monitorThread = std::thread([this]()
                              {
    int inotifyFd = inotify_init();
    if (inotifyFd < 0) {
      Logger::Error("[FileListener] Failed to initialize inotify.");
      return;
    }

    if (!fs::exists(watchedDirectory) || !fs::is_directory(watchedDirectory)) {
      Logger::Error("[FileListener] Directory does not exist or is not a directory: " + watchedDirectory);
      close(inotifyFd);
      return;
    }

    int watchDescriptor = inotify_add_watch(inotifyFd, watchedDirectory.c_str(), IN_CREATE | IN_DELETE | IN_MODIFY);
    if (watchDescriptor < 0) {
      Logger::Error("[FileListener] Failed to add inotify watch on directory: " + watchedDirectory);
      close(inotifyFd);
      return;
    }

    Logger::Info("[FileListener] File monitor started on: " + watchedDirectory);

    constexpr size_t eventSize = sizeof(struct inotify_event);
    constexpr size_t bufferSize = 1024 * (eventSize + 16);
    std::vector<char> buffer(bufferSize);

    while (running.load()) {
      int length = read(inotifyFd, buffer.data(), bufferSize);
      if (length < 0) {
        Logger::Error("[FileListener] Error reading inotify events.");
        break;
      }

    int i = 0;
    std::string oldFilePath;
    while (i < length) {
      struct inotify_event *event = reinterpret_cast<struct inotify_event *>(&buffer[i]);

      if (event->len > 0) {
        std::string filePath = watchedDirectory;
        if (event->len > 0 && event->name[0] != '\0' && 
            !(event->mask & IN_DELETE_SELF) && 
            !(event->mask & IN_ISDIR && filePath == watchedDirectory)) {
          filePath = fs::path(watchedDirectory) / event->name;
        }

        std::string eventType;

        if (event->mask & IN_CREATE)
          eventType = "created";
        else if (event->mask & IN_DELETE)
          eventType = "deleted";
        else if (event->mask & IN_DELETE_SELF) {
          eventType = "directory_deleted";
          running.store(false);
        }
        else if (event->mask & IN_MODIFY)
          eventType = "modified";
        else if (event->mask & IN_ATTRIB)
          eventType = "metadata_changed";
        else if (event->mask & IN_MOVED_FROM) {
          eventType = "moved_from";
          oldFilePath = filePath;
        }
        else if (event->mask & IN_MOVED_TO) {
          eventType = "moved_to";
          if (!oldFilePath.empty()) {
            Logger::Info("[FileListener] File renamed: " + oldFilePath + " -> " + filePath);
            eventCallback("renamed", oldFilePath + " -> " + filePath);
            oldFilePath.clear();
          } else {
            eventCallback(eventType, filePath);
          }
        }

        if (!eventType.empty()) {
          Logger::Info("[FileListener] File event detected: " + eventType + " - " + filePath);
          eventCallback(eventType, filePath);
        }
      }

      i += eventSize + event->len;
    }
  }

  inotify_rm_watch(inotifyFd, watchDescriptor);
  close(inotifyFd);
  Logger::Info("[FileListener] File monitor stopped on: " + watchedDirectory); });

  monitorThread.detach();
}

void FileListener::FileMonitor::Stop()
{
  running.store(false);
}
