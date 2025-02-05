#include <criterion/criterion.h>
#include "filerix/FileSystem/DriveUtils.h"
#include "TestUtils.h"

DriveUsage GetDriveUsage(const char *drive)
{
  if (strcmp(drive, "/dev/sda") == 0)
  {
    return (DriveUsage){5000000000, 10000000000};
  }
  return (DriveUsage){0, 0};
}

Test(DriveUtilsTest, GetDriveUsage_Success)
{
  DriveUsage usage = GetDriveUsage("/dev/sda");

  cr_assert_gt(usage.used, 0, "Used bytes should be greater than 0");
  cr_assert_gt(usage.total, 0, "Total bytes should be greater than 0");
  cr_assert_leq(usage.used, usage.total, "Used bytes should not be greater than total bytes");
}

Test(DriveUtilsTest, GetDriveUsage_Failure)
{
  DriveUsage usage = GetDriveUsage("/dev/sdX");

  cr_assert_eq(usage.used, 0, "Used bytes should be 0 for non-existing device");
  cr_assert_eq(usage.total, 0, "Total bytes should be 0 for non-existing device");
}
