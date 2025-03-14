#include "Listeners/DriveListener.h"
#include "Utils/Logger.h"

#include <errno.h>
#include <libudev.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

static bool running = false;
static pthread_t monitorThread;
static DriveEventCallback eventCallback = NULL;

void *DriveListener_MonitorThread(void *arg)
{
  Logger_Info("[gold]DriveListener[/gold] - Initializing drive monitor...");

  struct udev *udev = udev_new();
  if (!udev)
  {
    Logger_Error("[red]DriveListener[/red] - Failed to create udev context.");
    return NULL;
  }

  struct udev_monitor *monitor = udev_monitor_new_from_netlink(udev, "udev");
  if (!monitor)
  {
    Logger_Error("[red]DriveListener[/red] - Failed to create udev monitor.");
    udev_unref(udev);
    return NULL;
  }

  udev_monitor_filter_add_match_subsystem_devtype(monitor, "block", NULL);
  udev_monitor_enable_receiving(monitor);

  int fd = udev_monitor_get_fd(monitor);

  Logger_Info("[gold]DriveListener[/gold] - Drive monitor started.");

  while (running)
  {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    struct timeval timeout = {1, 0};

    int ret = select(fd + 1, &fds, NULL, NULL, &timeout);
    if (ret > 0 && FD_ISSET(fd, &fds))
    {
      struct udev_device *dev = udev_monitor_receive_device(monitor);
      if (dev)
      {
        const char *action = udev_device_get_action(dev);
        const char *devnode = udev_device_get_devnode(dev);

        if (action && devnode)
        {
          Logger_Info("[gold]DriveListener[/gold] - Drive event detected: %s - %s", action, devnode);
          if (eventCallback)
          {
            eventCallback(action, devnode);
          }
        }
        else
        {
          Logger_Warn("[gold]DriveListener[/gold] - Received event with missing data.");
        }

        udev_device_unref(dev);
      }
    }
    else if (ret < 0)
    {
      Logger_Error("[red]DriveListener[/red] - Error in select(): %s", strerror(errno));
    }
  }

  udev_monitor_unref(monitor);
  udev_unref(udev);

  Logger_Info("[gold]DriveListener[/gold] - Drive monitor stopped.");
  return NULL;
}

void DriveListener_Start(DriveEventCallback callback)
{
  if (running)
  {
    Logger_Warn("[gold]DriveListener[/gold] - Drive monitor is already running.");
    return;
  }

  Logger_Info("[gold]DriveListener[/gold] - Starting drive monitor...");

  running = true;
  eventCallback = callback;

  if (pthread_create(&monitorThread, NULL, DriveListener_MonitorThread, NULL) != 0)
  {
    Logger_Error("[red]DriveListener[/red] - Failed to create monitor thread: %s", strerror(errno));
    running = false;
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
  pthread_join(monitorThread, NULL);
  eventCallback = NULL;
  Logger_Info("[gold]DriveListener[/gold] - Drive monitor stopped.");
}
