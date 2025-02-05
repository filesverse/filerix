#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int CreateDirectory(const char *path)
{
  return mkdir(path, 0777) == 0;
}

Test(FileUtilsTest, GetFiles_Success)
{
  const char *testDir = "./TestDir";
  const char *testFile1 = "./TestDir/file1.txt";
  const char *testFile2 = "./TestDir/file2.txt";

  cr_assert(CreateDirectory(testDir), "Failed to create test directory");
  cr_assert(CreateFile(testFile1), "Failed to create test file1");
  cr_assert(CreateFile(testFile2), "Failed to create test file2");

  size_t count;
  struct FileInfo *files = GetFiles(testDir, &count);

  cr_assert_eq(count, 2, "Expected 2 files in directory");

  cr_assert_str_eq(files[0].name, "file1.txt", "First file name mismatch");
  cr_assert_str_eq(files[1].name, "file2.txt", "Second file name mismatch");

  Cleanup(testFile1);
  Cleanup(testFile2);
}

Test(FileUtilsTest, GetFiles_Failure, .init = setup_redirect, .exit_code = EXIT_FAILURE)
{
  const char *invalidDir = "./InvalidDir";
  size_t count;
  struct FileInfo *files = GetFiles(invalidDir, &count);

  cr_assert_null(files, "Expected null result for invalid directory");
  cr_assert_eq(count, 0, "Expected count to be 0 for invalid directory");
}
