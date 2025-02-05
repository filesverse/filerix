#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include "filerix/FileSystem/UserUtils.h"
#include "TestUtils.h"

#define TEST_FILE "./test_file_permissions.txt"
#define INVALID_FILE "./non_existent_file.txt"
#define MODE 0644

Test(UserUtils, ChangePermissions_Success, .init = setup_redirect)
{
  cr_assert(CreateFile(TEST_FILE), "Failed to create test file");
  cr_assert(ChangePermissions(TEST_FILE, MODE), "Failed to change file permissions");

  struct stat fileStat;
  cr_assert_eq(stat(TEST_FILE, &fileStat), 0, "Failed to retrieve file stats");
  cr_assert_eq(fileStat.st_mode & 0777, MODE, "File permissions do not match expected mode");

  Cleanup(TEST_FILE);
}

Test(UserUtils, ChangePermissions_Failure, .init = setup_redirect, .exit_code = EXIT_FAILURE)
{
  ChangePermissions(INVALID_FILE, MODE);
}
