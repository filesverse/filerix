#include "filerix/FileSystem/UserUtils.h"
#include "TestUtils.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/stat.h>
#include <cstdlib>
#include <filesystem>

const std::string filePath = "./test_file_permissions.txt";
const std::string invalidFilePath = "./non_existent_file.txt";
const int mode = 0644;

Test(UserUtilsTest, ChangePermissions_Success, .init = setup_redirect)
{
  cr_assert(CreateFile(filePath.c_str()), "Failed to create test file");

  cr_assert(UserUtils::ChangePermissions(filePath, mode), "Failed to change file permissions");

  struct stat fileStat;
  cr_assert_eq(stat(filePath.c_str(), &fileStat), 0, "Failed to retrieve file stats");
  cr_assert_eq(fileStat.st_mode & 0777, mode, "File permissions do not match expected mode");

  Cleanup(filePath.c_str());
}

Test(UserUtilsTest, ChangePermissions_Failure, .init = setup_redirect, .exit_code = EXIT_FAILURE)
{
  UserUtils::ChangePermissions(invalidFilePath, mode);
}
