#include "include/Listeners/FileListener.h"
#include "include/Utils/Logger.h"

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
            Logger::Error("Failed to initialize inotify.");
            return;
        }

        int watchDescriptor = inotify_add_watch(inotifyFd, watchedDirectory.c_str(), 
                                                IN_CREATE | IN_DELETE | IN_MODIFY);
        if (watchDescriptor < 0) {
            Logger::Error("Failed to add inotify watch on directory: " + watchedDirectory);
            close(inotifyFd);
            return;
        }

        Logger::Info("File monitor started on: " + watchedDirectory);

        constexpr size_t eventSize = sizeof(struct inotify_event);
        constexpr size_t bufferSize = 1024 * (eventSize + 16);
        std::vector<char> buffer(bufferSize);

        while (running.load()) {
            int length = read(inotifyFd, buffer.data(), bufferSize);
            if (length < 0) {
                Logger::Error("Error reading inotify events.");
                break;
            }

            int i = 0;
            while (i < length) {
                struct inotify_event *event = reinterpret_cast<struct inotify_event *>(&buffer[i]);

                if (event->len > 0) {
                    std::string filePath = watchedDirectory + "/" + event->name;
                    std::string eventType;

                    if (event->mask & IN_CREATE)
                        eventType = "created";
                    else if (event->mask & IN_DELETE)
                        eventType = "deleted";
                    else if (event->mask & IN_MODIFY)
                        eventType = "modified";

                    if (!eventType.empty()) {
                        Logger::Info("File event detected: " + eventType + " - " + filePath);
                        eventCallback(eventType, filePath);
                    }
                }

                i += eventSize + event->len;
            }
        }

        inotify_rm_watch(inotifyFd, watchDescriptor);
        close(inotifyFd);
        Logger::Info("File monitor stopped on: " + watchedDirectory); });

  monitorThread.detach();
}

void FileListener::FileMonitor::Stop()
{
  running.store(false);
}
