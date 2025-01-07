#include <string>
#include <vector>
#include <stdexcept>
#include <mntent.h>
#include <algorithm>
#include <sys/statvfs.h>
#include <sstream>
#include <iomanip>
#include "include/FileSystem/UserUtils.hpp"

namespace MountUtils
{
  std::vector<std::string> GetMountedDrives()
  {
    std::vector<std::string> drives;
    std::string user = UserUtils::GetUserName();
    std::string userMountPath = "/run/media/" + user;

    FILE *file = setmntent("/proc/mounts", "r");
    if (file)
    {
      struct mntent *mnt;
      while ((mnt = getmntent(file)))
      {
        std::string mountPoint = mnt->mnt_dir;
        if (mountPoint.find(userMountPath) == 0)
        {
          if (std::find(drives.begin(), drives.end(), mountPoint) == drives.end())
          {
            drives.push_back(mountPoint);
          }
        }
      }
      endmntent(file);
    }
    return drives;
  }

  // Convert bytes to a human-readable format (KB, MB, GB, TB)
  std::string ConvertBytesToHumanReadable(unsigned long long bytes)
  {
    const char *units[] = {"B", "KiB", "MiB", "GiB", "TiB"};
    double size = static_cast<double>(bytes);
    int unitIndex = 0;

    // Scale bytes to appropriate unit
    while (size >= 1024 && unitIndex < 4)
    {
      size /= 1024.0;
      unitIndex++;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unitIndex];
    return oss.str();
  }

  // Get both used space and total space on a drive, in human-readable format
  std::pair<std::string, std::string> GetDriveUsage(const std::string &drive)
  {
    struct statvfs stat;

    if (statvfs(drive.c_str(), &stat) != 0)
    {
      throw std::runtime_error("Unable to get drive information for " + drive);
    }

    unsigned long long totalSpace = stat.f_blocks * stat.f_frsize;
    unsigned long long usedSpace = (stat.f_blocks - stat.f_bfree) * stat.f_frsize;

    // Convert both used and total space to human-readable formats
    std::string usedSpaceStr = ConvertBytesToHumanReadable(usedSpace);
    std::string totalSpaceStr = ConvertBytesToHumanReadable(totalSpace);

    return {usedSpaceStr, totalSpaceStr};
  }
}
