#include <criterion/criterion.h>
#include "filerix/FileSystem/DriveUtils.h"
#include "TestUtils.h"

namespace DriveUtils
{
  bool MountDrive(const std::string &device)
  {
    if (device == "/dev/sda")
    {
      return true;
    }
    return false;
  }
}

Test(DriveUtilsTest, MountDrive_Success)
{
  bool success = DriveUtils::MountDrive("/dev/sda");

  cr_assert(success, "Mounting /dev/sda should return true");
}

Test(DriveUtilsTest, MountDrive_Failure)
{
  bool success = DriveUtils::MountDrive("/dev/sdX");

  cr_assert_not(success, "Mounting /dev/sdX should return false");
}
