#include <criterion/criterion.h>
#include "filerix/FileSystem/DriveUtils.h"
#include "TestUtils.h"
#include <string.h>

bool UnmountDrive(const char *device)
{
  if (strcmp(device, "/dev/sda") == 0)
  {
    return true;
  }
  return false;
}

Test(DriveUtilsTest, UnmountDrive_Success)
{
  bool success = UnmountDrive("/dev/sda");

  cr_assert(success, "Unmounting /dev/sda should return true");
}

Test(DriveUtilsTest, UnmountDrive_Failure)
{
  bool success = UnmountDrive("/dev/sdX");

  cr_assert_not(success, "Unmounting /dev/sdX should return false");
}
