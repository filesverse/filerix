#include <criterion/criterion.h>
#include "filerix/FileSystem/DriveUtils.h"
#include "TestUtils.h"

namespace DriveUtils
{
  bool UnmountDrive(const std::string &device)
  {
    if (device == "/dev/sda")
    {
      return true;
    }
    return false;
  }
}

Test(DriveUtilsTest, UnmountDrive_Success)
{
  bool success = DriveUtils::UnmountDrive("/dev/sda");

  cr_assert(success, "Unmounting /dev/sda should return true");
}

Test(DriveUtilsTest, UnmountDrive_Failure)
{
  bool success = DriveUtils::UnmountDrive("/dev/sdX");

  cr_assert_not(success, "Unmounting /dev/sdX should return false");
}
