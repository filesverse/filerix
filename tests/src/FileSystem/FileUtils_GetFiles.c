#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

Test(FileUtilsTest, GetFiles_Success)
{
  const char *testDir = "./TestDir_GetFiles";
  const char *testFile1 = "./TestDir/file1.txt";
  const char *testFile2 = "./TestDir/file2.txt";

  cr_assert(CreateDir(testDir), "Failed to create test directory");
  cr_assert(CreateFile(testFile1), "Failed to create test file1");
  cr_assert(CreateFile(testFile2), "Failed to create test file2");

  struct FileList files = GetFiles(testDir);

  cr_assert_str_eq(files.files[0].name, "file1.txt", "First file name mismatch");
  cr_assert_str_eq(files.files[1].name, "file2.txt", "Second file name mismatch");

  Cleanup(testFile1);
  Cleanup(testFile2);
  RemoveDir(testDir);
}

Test(FileUtilsTest, GetFiles_Failure, .init = SetupRedirect, .exit_code = EXIT_FAILURE)
{
  const char *invalidDir = "./InvalidDir";
  GetFiles(invalidDir);
}
