#include "Listeners/DriveListener.h"
#include "Utils/Logger.h"

#include <windows.h>
#include <dbt.h>
#include <stdbool.h>
#include <stdlib.h>

static bool running = false;
static HANDLE threadHandle = NULL;
static DriveEventCallback eventCallback = NULL;
static HWND hwnd = NULL;

LRESULT CALLBACK DriveListener_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_DEVICECHANGE)
  {
    PDEV_BROADCAST_HDR hdr = (PDEV_BROADCAST_HDR)lParam;
    if (wParam == DBT_DEVICEARRIVAL && hdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
    {
      PDEV_BROADCAST_VOLUME vol = (PDEV_BROADCAST_VOLUME)hdr;
      char driveLetter = 'A';

      for (int i = 0; i < 26; i++)
      {
        if (vol->dbcv_unitmask & (1 << i))
        {
          driveLetter = 'A' + i;
          break;
        }
      }

      char devnode[4] = {driveLetter, ':', '\\', '\0'};
      Logger_Info("[gold]DriveListener[/gold] - Drive plugged in: %s", devnode);
      if (eventCallback)
      {
        eventCallback("add", devnode);
      }
    }
    else if (wParam == DBT_DEVICEREMOVECOMPLETE && hdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
    {
      PDEV_BROADCAST_VOLUME vol = (PDEV_BROADCAST_VOLUME)hdr;
      char driveLetter = 'A';

      for (int i = 0; i < 26; i++)
      {
        if (vol->dbcv_unitmask & (1 << i))
        {
          driveLetter = 'A' + i;
          break;
        }
      }

      char devnode[4] = {driveLetter, ':', '\\', '\0'};
      Logger_Info("[gold]DriveListener[/gold] - Drive removed: %s", devnode);
      if (eventCallback)
      {
        eventCallback("remove", devnode);
      }
    }
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

DWORD WINAPI DriveListener_Thread(LPVOID lpParam)
{
  Logger_Info("[gold]DriveListener[/gold] - Initializing drive monitor...");

  WNDCLASS wc = {0};
  wc.lpfnWndProc = DriveListener_WindowProc;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = "DriveListenerClass";

  if (!RegisterClass(&wc))
  {
    Logger_Error("[red]DriveListener[/red] - Failed to register window class.");
    return 1;
  }

  hwnd = CreateWindow("DriveListenerClass", "DriveListener", 0, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
  if (!hwnd)
  {
    Logger_Error("[red]DriveListener[/red] - Failed to create window.");
    return 1;
  }

  DEV_BROADCAST_DEVICEINTERFACE filter = {0};
  filter.dbcc_size = sizeof(filter);
  filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
  filter.dbcc_classguid = GUID_DEVINTERFACE_VOLUME;

  HDEVNOTIFY hNotify = RegisterDeviceNotification(hwnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);
  if (!hNotify)
  {
    Logger_Error("[red]DriveListener[/red] - Failed to register device notification.");
    return 1;
  }

  Logger_Info("[gold]DriveListener[/gold] - Drive monitor started.");

  running = true;
  MSG msg;
  while (running)
  {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    Sleep(100);
  }

  UnregisterDeviceNotification(hNotify);
  DestroyWindow(hwnd);
  hwnd = NULL;

  Logger_Info("[gold]DriveListener[/gold] - Drive monitor stopped.");
  return 0;
}

void DriveListener_Start(DriveEventCallback callback)
{
  if (running)
  {
    Logger_Warn("[gold]DriveListener[/gold] - Drive monitor is already running.");
    return;
  }

  Logger_Info("[gold]DriveListener[/gold] - Starting drive monitor...");
  eventCallback = callback;

  threadHandle = CreateThread(NULL, 0, DriveListener_Thread, NULL, 0, NULL);
  if (!threadHandle)
  {
    Logger_Error("[red]DriveListener[/red] - Failed to create monitor thread.");
    return;
  }
}

void DriveListener_Stop()
{
  if (!running)
  {
    Logger_Warn("[gold]DriveListener[/gold] - Drive monitor is not running.");
    return;
  }

  Logger_Info("[gold]DriveListener[/gold] - Stopping drive monitor...");
  running = false;

  if (threadHandle)
  {
    WaitForSingleObject(threadHandle, INFINITE);
    CloseHandle(threadHandle);
    threadHandle = NULL;
  }

  eventCallback = NULL;
  Logger_Info("[gold]DriveListener[/gold] - Drive monitor stopped.");
}
