#include "Listeners/FileListener.h"
#include "Utils/Logger.h"

#include <windows.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

static bool running = false;
static pthread_t monitorThread;
static FileEventCallback eventCallback = NULL;
static char watchedDirectory[PATH_MAX];
static HANDLE directoryHandle = INVALID_HANDLE_VALUE;
static OVERLAPPED overlap;

void *FileListener_MonitorThread(void *arg)
{
  Logger_Info("[gold]FileListener[/gold] - Initializing file monitor...");

  directoryHandle = CreateFile(
      watchedDirectory,
      FILE_LIST_DIRECTORY,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      NULL,
      OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
      NULL);

  if (directoryHandle == INVALID_HANDLE_VALUE)
  {
    Logger_Error("[red]FileListener[/red] - Failed to open directory for monitoring: %s", strerror(errno));
    return NULL;
  }

  Logger_Info("[gold]FileListener[/gold] - File monitor started on: %s", watchedDirectory);

  char buffer[1024];
  DWORD bytesReturned;
  FILE_NOTIFY_INFORMATION *notifyInfo = (FILE_NOTIFY_INFORMATION *)buffer;

  while (running)
  {
    Logger_Info("[gold]FileListener[/gold] - Waiting for file events...");

    // Read the directory changes
    if (ReadDirectoryChangesW(
            directoryHandle,
            buffer,
            sizeof(buffer),
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME |
                FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_ATTRIBUTES |
                FILE_NOTIFY_CHANGE_SIZE |
                FILE_NOTIFY_CHANGE_LAST_WRITE |
                FILE_NOTIFY_CHANGE_CREATION,
            &bytesReturned,
            &overlap,
            NULL) == 0)
    {
      Logger_Error("[red]FileListener[/red] - Error reading directory changes: %s", strerror(errno));
      break;
    }

    do
    {
      wchar_t filePathW[PATH_MAX];
      wcscpy(filePathW, notifyInfo->FileName);
      char filePath[PATH_MAX];
      if (wcstombs(filePath, filePathW, PATH_MAX) == (size_t)-1)
      {
        Logger_Error("[red]FileListener[/red] - Failed to convert wide string to multibyte: %s", strerror(errno));
        continue;
      }

      char eventType[32] = {0};

      switch (notifyInfo->Action)
      {
      case FILE_ACTION_ADDED:
        strcpy(eventType, "created");
        break;
      case FILE_ACTION_REMOVED:
        strcpy(eventType, "deleted");
        break;
      case FILE_ACTION_MODIFIED:
        strcpy(eventType, "modified");
        break;
      case FILE_ACTION_RENAMED_OLD_NAME:
        strcpy(eventType, "moved_from");
        break;
      case FILE_ACTION_RENAMED_NEW_NAME:
        strcpy(eventType, "moved_to");
        break;
      }

      if (eventType[0] != '\0')
      {
        Logger_Info("[gold]FileListener[/gold] - File event detected: %s - %s", eventType, filePath);
        if (eventCallback)
        {
          eventCallback(eventType, filePath);
        }
      }

      notifyInfo = (FILE_NOTIFY_INFORMATION *)((char *)notifyInfo + notifyInfo->NextEntryOffset);
    } while (notifyInfo->NextEntryOffset > 0);
  }

  CloseHandle(directoryHandle);
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
