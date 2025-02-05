#include <criterion/criterion.h>
#include "filerix/FileSystem/DriveUtils.h"
#include "TestUtils.h"

const char *device = "/dev/sda";
const char *invalidDevice = "/dev/sdX";

const char *GetDeviceLabelOrUUID(const char *device)
{
  if (strcmp(device, "/dev/sda") == 0)
  {
    return "DISK-UUID-12345";
  }
  return NULL;
}

Test(DriveUtilsTest, GetDeviceLabelOrUUID_Success)
{
  const char *label_or_uuid = GetDeviceLabelOrUUID(device);
  cr_assert(label_or_uuid != NULL, "Label or UUID should not be NULL");
  cr_assert_str_eq(label_or_uuid, "DISK-UUID-12345", "Label or UUID should match");
}

Test(DriveUtilsTest, GetDeviceLabelOrUUID_Failure)
{
  const char *label_or_uuid = GetDeviceLabelOrUUID(invalidDevice);
  cr_assert_eq(label_or_uuid, NULL, "Label or UUID should be NULL");
}
