#include <criterion/criterion.h>
#include "filerix/FileSystem/DriveUtils.h"
#include "TestUtils.h"
#include <vector>
#include <string>

namespace DriveUtils
{
  std::vector<DriveInfo> GetDrives()
  {
    return {{status : "mounted", mountPoint : "/"}, {status : "unmounted", mountPoint : "/dev/sdb"}};
  }
}

Test(DriveUtilsTest, GetDrives_Success)
{
  auto drives = DriveUtils::GetDrives();

  cr_assert(!drives.empty(), "Drives list should not be empty");
  cr_assert_eq(drives.size(), 2, "There should be 2 drives");

  cr_assert_str_eq(drives[0].status.c_str(), "mounted", "First drive status should be 'mounted'");
  cr_assert_str_eq(drives[0].mountPoint.c_str(), "/", "First drive mount point should be '/'");
}

Test(DriveUtilsTest, GetDrives_Empty)
{
  auto drives = std::vector<DriveInfo>{};

  cr_assert(drives.empty(), "Drives list should be empty");
}
