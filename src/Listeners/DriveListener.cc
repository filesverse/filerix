#include "Listeners/DriveListener.h"
#include "Utils/Logger.h"

DriveListener::DriveMonitor::DriveMonitor() : running(false) {}

DriveListener::DriveMonitor::~DriveMonitor()
{
  Stop();
}

void DriveListener::DriveMonitor::Start(std::function<void(std::string, std::string)> callback)
{
  if (running.load())
    return;

  running.store(true);
  eventCallback = callback;

  monitorThread = std::thread([this]()
                              {
        struct udev *udev = udev_new();
        if (!udev) {
            Logger::Error("[DriveListener] Failed to create udev context.");
            return;
        }

        struct udev_monitor *monitor = udev_monitor_new_from_netlink(udev, "udev");
        if (!monitor) {
            Logger::Error("[DriveListener] Failed to create udev monitor.");
            udev_unref(udev);
            return;
        }

        udev_monitor_filter_add_match_subsystem_devtype(monitor, "block", NULL);
        udev_monitor_enable_receiving(monitor);

        int fd = udev_monitor_get_fd(monitor);

        Logger::Info("[DriveListener] Drive monitor started.");

        while (running.load()) {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            struct timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            int ret = select(fd + 1, &fds, NULL, NULL, &timeout);
            if (ret > 0 && FD_ISSET(fd, &fds)) {
                struct udev_device *dev = udev_monitor_receive_device(monitor);
                if (dev) {
                    const char *action = udev_device_get_action(dev);
                    const char *devnode = udev_device_get_devnode(dev);

                    if (action && devnode) {
                        Logger::Info("[DriveListener] Drive event detected: " + std::string(action) + " - " + std::string(devnode));
                        eventCallback(action, devnode);
                    }

                    udev_device_unref(dev);
                }
            }
        }

        udev_monitor_unref(monitor);
        udev_unref(udev);
        Logger::Info("[DriveListener] Drive monitor stopped."); });

  monitorThread.detach();
}

void DriveListener::DriveMonitor::Stop()
{
  running.store(false);
}
