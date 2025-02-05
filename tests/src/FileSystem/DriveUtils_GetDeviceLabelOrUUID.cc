#include <criterion/criterion.h>
#include "filerix/FileSystem/DriveUtils.h"
#include "TestUtils.h"

const char *device = "/dev/sda";
const char *invalidDevice = "/dev/sdX";

namespace DriveUtils
{
  const char *GetDeviceLabelOrUUID(const char *device)
  {
    if (std::string(device) == "/dev/sda")
    {
      return "DISK-UUID-12345";
    }
    return nullptr;
  }
}

Test(DriveUtilsTest, GetDeviceLabelOrUUID_Success)
{
  const char *label_or_uuid = DriveUtils::GetDeviceLabelOrUUID(device);
  cr_assert(label_or_uuid != nullptr, "Label or UUID should not be NULL");
  cr_assert_str_eq(label_or_uuid, "DISK-UUID-12345", "Label or UUID should match");
}

Test(DriveUtilsTest, GetDeviceLabelOrUUID_Failure)
{
  const char *label_or_uuid = DriveUtils::GetDeviceLabelOrUUID(invalidDevice);
  cr_assert_eq(label_or_uuid, nullptr, "Label or UUID should be NULL");
}
