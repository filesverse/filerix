#include <stdio.h>
#include "libfm/FileSystem/DriveUtils.h"

int main()
{
  const char *device = "/dev/sdX";

  DriveUsage usage = GetDriveUsage(device);

  if (usage.used || usage.total)
  {
    printf("Used space: %llu bytes\n", usage.used);
    printf("Total space: %llu bytes\n", usage.total);
  }
  else
  {
    fprintf(stderr, "Failed to get drive usage.\n");
  }

  return 0;
}
