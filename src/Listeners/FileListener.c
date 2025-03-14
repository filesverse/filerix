#include "Listeners/FileListener.h"
#include "Utils/Logger.h"

#include <sys/inotify.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

static bool running = false;
static pthread_t monitorThread;
static FileEventCallback eventCallback = NULL;
static char watchedDirectory[PATH_MAX];

void *FileListener_MonitorThread(void *arg)
{
  Logger_Info("[gold]FileListener[/gold] - Initializing file monitor...");

  int inotifyFd = inotify_init();
  if (inotifyFd < 0)
  {
    Logger_Error("[red]FileListener[/red] - Failed to initialize inotify: %s", strerror(errno));
    return NULL;
  }

  struct stat dirStat;
  if (stat(watchedDirectory, &dirStat) < 0 || !S_ISDIR(dirStat.st_mode))
  {
    Logger_Error("[red]FileListener[/red] - Directory does not exist or is not a directory: %s", watchedDirectory);
    close(inotifyFd);
    return NULL;
  }

  int watchDescriptor = inotify_add_watch(inotifyFd, watchedDirectory, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO);
  if (watchDescriptor < 0)
  {
    Logger_Error("[red]FileListener[/red] - Failed to add inotify watch on directory: %s", watchedDirectory);
    close(inotifyFd);
    return NULL;
  }

  Logger_Info("[gold]FileListener[/gold] - File monitor started on: %s", watchedDirectory);

  const size_t eventSize = sizeof(struct inotify_event);
  const size_t bufferSize = 1024 * (eventSize + 16);
  char buffer[bufferSize];

  char oldFilePath[PATH_MAX] = {0};

  while (running)
  {
    int length = read(inotifyFd, buffer, bufferSize);
    if (length < 0)
    {
      Logger_Error("[red]FileListener[/red] - Error reading inotify events: %s", strerror(errno));
      break;
    }

    int i = 0;
    while (i < length)
    {
      struct inotify_event *event = (struct inotify_event *)&buffer[i];

      if (event->len > 0)
      {
        char filePath[PATH_MAX];
        if (strlen(watchedDirectory) + strlen(event->name) + 1 >= PATH_MAX)
        {
          Logger_Error("[FileListener] Path too long: %s/%s", watchedDirectory, event->name);
          return NULL;
        }
        snprintf(filePath, sizeof(filePath), "%s/%s", watchedDirectory, event->name);

        char eventType[32] = {0};

        if (event->mask & IN_CREATE)
        {
          strcpy(eventType, "created");
        }
        else if (event->mask & IN_DELETE)
        {
          strcpy(eventType, "deleted");
        }
        else if (event->mask & IN_MODIFY)
        {
          strcpy(eventType, "modified");
        }
        else if (event->mask & IN_MOVED_FROM)
        {
          strcpy(eventType, "moved_from");
          strncpy(oldFilePath, filePath, PATH_MAX);
        }
        else if (event->mask & IN_MOVED_TO)
        {
          strcpy(eventType, "moved_to");
          if (oldFilePath[0] != '\0')
          {
            char renamedEvent[PATH_MAX * 2];
            if (strlen(oldFilePath) + strlen(filePath) + 4 >= sizeof(renamedEvent))
            {
              Logger_Error("[FileListener] Renamed event too long: %s -> %s", oldFilePath, filePath);
              return NULL;
            }
            snprintf(renamedEvent, sizeof(renamedEvent), "%s -> %s", oldFilePath, filePath);
            Logger_Info("[gold]FileListener[/gold] - File renamed: %s", renamedEvent);
            if (eventCallback)
            {
              eventCallback("renamed", renamedEvent);
            }
            oldFilePath[0] = '\0';
          }
          else
          {
            if (eventCallback)
            {
              eventCallback(eventType, filePath);
            }
          }
        }

        if (eventType[0] != '\0')
        {
          Logger_Info("[gold]FileListener[/gold] - File event detected: %s - %s", eventType, filePath);
          if (eventCallback)
          {
            eventCallback(eventType, filePath);
          }
        }
      }

      i += eventSize + event->len;
    }
  }

  inotify_rm_watch(inotifyFd, watchDescriptor);
  close(inotifyFd);
  Logger_Info("[gold]FileListener[/gold] - File monitor stopped on: %s", watchedDirectory);
  return NULL;
}

void FileListener_Start(const char *directory, FileEventCallback callback)
{
  if (running)
  {
    Logger_Warn("[gold]FileListener[/gold] - File monitor is already running.");
    return;
  }

  Logger_Info("[gold]FileListener[/gold] - Starting file monitor for directory: %s", directory);

  running = true;
  eventCallback = callback;
  strncpy(watchedDirectory, directory, PATH_MAX);

  if (pthread_create(&monitorThread, NULL, FileListener_MonitorThread, NULL) != 0)
  {
    Logger_Error("[red]FileListener[/red] - Failed to create monitor thread: %s", strerror(errno));
    running = false;
  }
}

void FileListener_Stop()
{
  if (!running)
  {
    Logger_Warn("[gold]FileListener[/gold] - File monitor is not running.");
    return;
  }

  Logger_Info("[gold]FileListener[/gold] - Stopping file monitor...");
  running = false;
  pthread_join(monitorThread, NULL);
  Logger_Info("[gold]FileListener[/gold] - File monitor stopped.");
}
