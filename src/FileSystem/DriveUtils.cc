#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <mntent.h>
#include <algorithm>
#include <sys/statvfs.h>
#include <sys/mount.h>
#include <filesystem>
#include <unistd.h>
#include <iostream>
#include <regex>
#include <cstdlib>
#include "include/FileSystem/UserUtils.hpp"
#include "include/FileSystem/DriveUtils.hpp"

namespace DriveUtils
{
  namespace fs = std::filesystem;

  std::string GetMountPoint(const std::string &device)
  {
    FILE *file = setmntent("/proc/mounts", "r");
    if (!file)
    {
      throw std::runtime_error("Unable to read /proc/mounts");
    }

    struct mntent *mnt;
    while ((mnt = getmntent(file)))
    {
      if (mnt->mnt_fsname == device)
      {
        endmntent(file);
        return mnt->mnt_dir;
      }
    }
    endmntent(file);
    throw std::runtime_error("Device " + device + " is not mounted or does not exist.");
  }

  std::vector<DriveInfo> GetDrives()
  {
    std::vector<DriveInfo> drives;
    std::map<std::string, std::string> mountPoints;

    FILE *file = setmntent("/proc/mounts", "r");
    if (!file)
    {
      throw std::runtime_error("Unable to read /proc/mounts");
    }

    struct mntent *mnt;
    while ((mnt = getmntent(file)))
    {
      std::string device = mnt->mnt_fsname;
      std::string mountPoint = mnt->mnt_dir;

      if (device.find("/dev/sd") == 0 || device.find("/dev/nvme") == 0)
      {
        mountPoints[device] = mountPoint;
      }
    }
    endmntent(file);

    std::regex partitionRegex(R"((/dev/[a-zA-Z]+[0-9]+))");

    std::string hostDrive;

    for (const auto &entry : std::filesystem::directory_iterator("/dev"))
    {
      std::string devicePath = entry.path().string();

      if (devicePath.find("/dev/sd") == 0 || devicePath.find("/dev/nvme") == 0)
      {
        DriveInfo drive;

        std::smatch match;
        if (std::regex_search(devicePath, match, partitionRegex))
        {
          drive.device = devicePath.substr(0, devicePath.find_last_not_of("0123456789") + 1);
          drive.partition = devicePath;
        }
        else
        {
          drive.device = devicePath;
          drive.partition = "";
        }

        auto it = mountPoints.find(devicePath);
        if (it != mountPoints.end())
        {
          drive.status = "mounted";
          drive.mountPoint = it->second;
        }
        else
        {
          drive.status = "unmounted";
          drive.mountPoint = "";
        }

        drive.unmountable = (drive.mountPoint == "/" || drive.mountPoint == "/home" || drive.mountPoint == "/boot" || drive.mountPoint == "/boot/efi");

        if (drive.mountPoint == "/boot" || drive.mountPoint == "/boot/efi")
        {
          continue;
        }

        if (drive.mountPoint == "/home" || drive.mountPoint == "/")
        {
          hostDrive = drive.device;
          drive.mountPoint = "/";
        }

        drives.push_back(drive);
      }
    }

    std::sort(drives.begin(), drives.end(), [&hostDrive](const DriveInfo &a, const DriveInfo &b)
              {
        if (a.device == hostDrive) return true;
        if (b.device == hostDrive) return false;
        return a.device < b.device; });

    return drives;
  }

  std::pair<unsigned long long, unsigned long long> GetDriveUsage(const std::string &drive)
  {
    // Get the mount point for the device (e.g., /dev/sda1)
    std::string mountPoint = GetMountPoint(drive);

    struct statvfs stat;
    if (statvfs(mountPoint.c_str(), &stat) != 0)
    {
      throw std::runtime_error("Unable to get drive information for " + drive);
    }

    // Calculate total space and used space
    unsigned long long totalSpace = stat.f_blocks * stat.f_frsize;
    unsigned long long usedSpace = (stat.f_blocks - stat.f_bfree) * stat.f_frsize;

    return {usedSpace, totalSpace};
  }

  std::string GetDeviceLabelOrUUID(const std::string &device)
  {
    std::ostringstream command;
    command << "blkid -o value -s LABEL " << device << " 2>/dev/null";

    FILE *pipe = popen(command.str().c_str(), "r");
    if (!pipe)
    {
      perror("Failed to retrieve device label");
      return {};
    }

    char buffer[128];
    std::string label;
    while (fgets(buffer, sizeof(buffer), pipe))
    {
      label += buffer;
    }
    pclose(pipe);

    if (label.empty())
    {
      command.str("");
      command << "blkid -o value -s UUID " << device << " 2>/dev/null";

      pipe = popen(command.str().c_str(), "r");
      if (!pipe)
      {
        perror("Failed to retrieve device UUID");
        return {};
      }

      while (fgets(buffer, sizeof(buffer), pipe))
      {
        label += buffer;
      }
      pclose(pipe);
    }

    label.erase(std::remove(label.begin(), label.end(), '\n'), label.end());
    return label;
  }

  bool MountDrive(const std::string &device)
  {
    std::string command = "pkexec mount " + device;

    int ret = system(command.c_str());

    if (ret != 0)
    {
      std::cerr << "Failed to mount " << device << std::endl;
      return false;
    }

    std::cout << "Drive " << device << " mounted successfully." << std::endl;
    return true;
  }

  bool UnmountDrive(const std::string &device)
  {
    std::string command = "pkexec umount " + device;

    int ret = system(command.c_str());

    if (ret != 0)
    {
      std::cerr << "Failed to unmount " << device << std::endl;
      return false;
    }

    std::cout << "Drive " << device << " unmounted successfully." << std::endl;
    return true;
  }
}
