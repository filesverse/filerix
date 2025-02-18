#include "FileSystem/UserUtils.h"
#include "FileSystem/DriveUtils.h"
#include "Utils/Logger.h"

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
#include <future>
#include <fstream>

namespace DriveUtils
{
  namespace fs = std::filesystem;

  std::string GetMountPoint(const std::string &device)
  {
    Logger::Info("[DriveUtils] Attempting to get mount point for device: " + device);

    try
    {
      FILE *file = setmntent("/proc/mounts", "r");
      if (!file)
      {
        Logger::Error("[DriveUtils] Unable to read /proc/mounts");
      }

      struct mntent *mnt;
      while ((mnt = getmntent(file)))
      {
        if (mnt->mnt_fsname == device)
        {
          endmntent(file);
          Logger::Info("[DriveUtils] Mount point for " + device + " is " + mnt->mnt_dir);
          return mnt->mnt_dir;
        }
      }
      endmntent(file);
      Logger::Error("[DriveUtils] Device " + device + " is not mounted or does not exist.");
    }
    catch (const std::exception &e)
    {
      Logger::Error("[DriveUtils] failed to get mount point for device " + device + ": " + e.what());
    }
  }

  std::vector<DriveInfo> GetDrives()
  {
    Logger::Info("[DriveUtils] Getting list of valid drives with mountable partitions...");

    std::vector<DriveInfo> drives;
    std::map<std::string, std::pair<std::string, std::string>> mountPoints;
    std::string hostDrive;

    try
    {
      FILE *file = setmntent("/proc/mounts", "r");
      if (!file)
      {
        Logger::Error("[DriveUtils] Unable to read /proc/mounts");
      }

      struct mntent *mnt;
      while ((mnt = getmntent(file)))
      {
        std::string device = mnt->mnt_fsname;
        std::string mountPoint = mnt->mnt_dir;
        std::string fsType = mnt->mnt_type;

        if (device.find("/dev/") == 0)
        {
          mountPoints[device] = {mountPoint, fsType};

          if (mountPoint == "/")
          {
            hostDrive = device;
          }
        }
      }
      endmntent(file);

      std::regex driveRegex(R"(^/dev/(sd[a-z]+|nvme[0-9]+n[0-9]+)$)");

      std::regex partitionRegex(R"(^/dev/(sd[a-z]+[0-9]+|nvme[0-9]+n[0-9]+p[0-9]+)$)");

      for (const auto &entry : std::filesystem::directory_iterator("/sys/class/block"))
      {
        std::string deviceName = entry.path().filename().string();
        std::string devicePath = "/dev/" + deviceName;

        std::string ueventFile = entry.path().string() + "/device/uevent";
        if (std::filesystem::exists(ueventFile))
        {
          std::ifstream ueventStream(ueventFile);
          std::string line;
          while (std::getline(ueventStream, line))
          {
            if (line.find("DEVTYPE=disk") != std::string::npos && line.find("ID_BUS=usb") != std::string::npos)
            {
              Logger::Info("[DriveUtils] Skipping USB device: " + devicePath);
              goto continue_loop;
            }
          }
        }

        if (std::regex_match(devicePath, partitionRegex))
        {
          auto it = mountPoints.find(devicePath);
          DriveInfo drive;
          drive.device = devicePath;
          drive.partition = devicePath;

          if (it != mountPoints.end())
          {
            drive.status = "mounted";
            drive.mountPoint = it->second.first;
            drive.fsType = it->second.second;

            if ((drive.fsType == "vfat" && drive.mountPoint == "/boot/efi") ||
                drive.mountPoint == "/boot" ||
                drive.mountPoint == "/home")
            {
              Logger::Info("[DriveUtils] Skipping partition: " + drive.device + " (EFI, /boot, or /home)");
              continue;
            }

            Logger::Info("[DriveUtils] Drive " + drive.device + " is mounted at " + drive.mountPoint);
          }
          else
          {
            drive.status = "unmounted";
            drive.mountPoint = "";
            Logger::Info("[DriveUtils] Drive " + drive.device + " is unmounted.");
          }

          if (drive.device == hostDrive)
          {
            drive.unmountable = false;
          }
          else
          {
            drive.unmountable = true;
          }
          drives.push_back(drive);
        }

      continue_loop:
        continue;
      }

      if (!hostDrive.empty())
      {
        Logger::Info("[DriveUtils] Adding host drive: " + hostDrive);
        DriveInfo hostDriveInfo;
        hostDriveInfo.device = hostDrive;
        hostDriveInfo.partition = hostDrive;
        hostDriveInfo.mountPoint = "/";
        hostDriveInfo.status = "mounted";
        hostDriveInfo.fsType = mountPoints[hostDrive].second;
        hostDriveInfo.unmountable = false;
        drives.push_back(hostDriveInfo);
      }

      std::sort(drives.begin(), drives.end(), [&hostDrive](const DriveInfo &a, const DriveInfo &b)
                {
            if (a.device == hostDrive)
                return true;
            if (b.device == hostDrive)
                return false;
            return a.device < b.device; });

      Logger::Info("[DriveUtils] Drives sorted.");
    }
    catch (const std::exception &e)
    {
      Logger::Error("[DriveUtils] Error retrieving drives: " + std::string(e.what()));
    }

    return drives;
  }

  DriveUsage GetDriveUsage(const std::string &drive)
  {
    Logger::Info("[DriveUtils] Getting drive usage for: " + drive);

    try
    {
      std::string mountPoint = GetMountPoint(drive);

      struct statvfs stat;
      if (statvfs(mountPoint.c_str(), &stat) != 0)
      {
        Logger::Error("[DriveUtils] Unable to get drive information for " + drive);
      }

      unsigned long long totalSpace = stat.f_blocks * stat.f_frsize;
      unsigned long long usedSpace = (stat.f_blocks - stat.f_bfree) * stat.f_frsize;

      Logger::Info("[DriveUtils] Drive usage for " + drive + ": " + std::to_string(usedSpace) + " / " + std::to_string(totalSpace));

      return {usedSpace, totalSpace};
    }
    catch (const std::exception &e)
    {
      Logger::Error("[DriveUtils] failed to get drive usage for " + drive + ": " + e.what());
    }
  }

  std::string GetDeviceLabelOrUUID(const std::string &device)
  {
    Logger::Info("[DriveUtils] Getting label or UUID for device: " + device);

    try
    {
      for (const auto &entry : fs::directory_iterator("/dev/disk/by-label"))
      {
        if (fs::canonical(entry.path()) == fs::canonical(device))
        {
          Logger::Info("[DriveUtils] Found label for " + device + ": " + entry.path().filename().string());
          return entry.path().filename().string();
        }
      }

      for (const auto &entry : fs::directory_iterator("/dev/disk/by-uuid"))
      {
        if (fs::canonical(entry.path()) == fs::canonical(device))
        {
          Logger::Info("[DriveUtils] Found UUID for " + device + ": " + entry.path().filename().string());
          return entry.path().filename().string();
        }
      }
    }
    catch (const std::exception &e)
    {
      Logger::Error("[DriveUtils] Error getting label or UUID for device " + device + ": " + e.what());
      return {};
    }

    Logger::Warn("[DriveUtils] No label or UUID found for device: " + device);
    return {};
  }

  bool MountDrive(const std::string &device)
  {
    Logger::Info("[DriveUtils] Attempting to mount drive: " + device);

    try
    {
      std::string command = "pkexec mount " + device;

      int ret = system(command.c_str());

      if (ret != 0)
      {
        Logger::Error("[DriveUtils] Failed to mount " + device);
        return false;
      }

      Logger::Info("[DriveUtils] Drive " + device + " mounted successfully.");
      return true;
    }
    catch (const std::exception &e)
    {
      Logger::Error("[DriveUtils] Error mounting drive " + device + ": " + e.what());
      return false;
    }
  }

  bool UnmountDrive(const std::string &device)
  {
    Logger::Info("[DriveUtils] Attempting to unmount drive: " + device);

    try
    {
      std::string command = "pkexec umount " + device;

      int ret = system(command.c_str());

      if (ret != 0)
      {
        Logger::Error("[DriveUtils] Failed to unmount " + device);
        return false;
      }

      Logger::Info("[DriveUtils] Drive " + device + " unmounted successfully.");
      return true;
    }
    catch (const std::exception &e)
    {
      Logger::Error("[DriveUtils] Error unmounting drive " + device + ": " + e.what());
      return false;
    }
  }
}
