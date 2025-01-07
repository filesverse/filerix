#ifndef MOUNTUTILS_H
#define MOUNTUTILS_H

#include <string>
#include <vector>
#include <utility>

namespace MountUtils
{
  /**
   * @brief Retrieves a list of mounted drives for the current user.
   *
   * @return A vector of strings containing paths to the mounted drives.
   */
  std::vector<std::string> GetMountedDrives();

  /**
   * @brief Converts a byte value into a human-readable format (e.g., KB, MB, GB).
   *
   * @param bytes The size in bytes.
   * @return A string representing the size in human-readable format.
   */
  std::string ConvertBytesToHumanReadable(unsigned long long bytes);

  /**
   * @brief Retrieves the used and total space for a specified drive.
   *
   * @param drive The path to the drive.
   * @return A pair of strings, where the first is the used space and the second is the total space.
   */
  std::pair<std::string, std::string> GetDriveUsage(const std::string &drive);
}

#endif // MOUNTUTILS_H
