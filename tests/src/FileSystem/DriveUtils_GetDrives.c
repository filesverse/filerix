#include <criterion/criterion.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
  char status[20];
  char mountPoint[20];
} DriveInfo;

DriveInfo *GetDrives(int *size)
{
  *size = 2;
  DriveInfo *drives = malloc(sizeof(DriveInfo) * (*size));

  strcpy(drives[0].status, "mounted");
  strcpy(drives[0].mountPoint, "/");

  strcpy(drives[1].status, "unmounted");
  strcpy(drives[1].mountPoint, "/dev/sdb");

  return drives;
}

Test(DriveUtilsTest, GetDrives_Success)
{
  int size;
  DriveInfo *drives = GetDrives(&size);

  cr_assert_not_null(drives, "Drives should not be NULL");
  cr_assert_eq(size, 2, "There should be 2 drives");

  cr_assert_str_eq(drives[0].status, "mounted", "First drive status should be 'mounted'");
  cr_assert_str_eq(drives[0].mountPoint, "/", "First drive mount point should be '/'");

  free(drives);
}

Test(DriveUtilsTest, GetDrives_Empty)
{
  int size = 0;
  DriveInfo *drives = NULL;

  cr_assert_null(drives, "Drives should be NULL for an empty list");
  cr_assert_eq(size, 0, "Size of empty drives list should be 0");
}
