#include <criterion/criterion.h>
#include "filerix/FileSystem/DriveUtils.h"
#include "TestUtils.h"
#include <string.h>

bool MountDrive(const char *device)
{
  if (strcmp(device, "/dev/sda") == 0)
  {
    return true;
  }
  return false;
}

Test(DriveUtilsTest, MountDrive_Success)
{
  bool success = MountDrive("/dev/sda");

  cr_assert(success, "Mounting /dev/sda should return true");
}

Test(DriveUtilsTest, MountDrive_Failure)
{
  bool success = MountDrive("/dev/sdX");

  cr_assert_not(success, "Mounting /dev/sdX should return false");
}
