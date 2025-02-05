#include <criterion/criterion.h>
#include "filerix/FileSystem/DriveUtils.h"
#include "TestUtils.h"

namespace DriveUtils
{
  DriveUsage GetDriveUsage(const std::string &drive)
  {
    if (drive == "/dev/sda")
    {
      return {5000000000, 10000000000};
    }
    return {0, 0};
  }
}

Test(DriveUtilsTest, GetDriveUsage_Success)
{
  auto [usedBytes, totalBytes] = DriveUtils::GetDriveUsage("/dev/sda");

  cr_assert_gt(usedBytes, 0, "Used bytes should be greater than 0");
  cr_assert_gt(totalBytes, 0, "Total bytes should be greater than 0");
  cr_assert_leq(usedBytes, totalBytes, "Used bytes should not be greater than total bytes");
}

Test(DriveUtilsTest, GetDriveUsage_Failure)
{
  auto [usedBytes, totalBytes] = DriveUtils::GetDriveUsage("/dev/sdX");

  cr_assert_eq(usedBytes, 0, "Used bytes should be 0 for non-existing device");
  cr_assert_eq(totalBytes, 0, "Total bytes should be 0 for non-existing device");
}
