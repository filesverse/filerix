#include <criterion/criterion.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"

const char *testDir = "./test_search_files";
const char *file1 = "./test_search_files/project1.txt";
const char *file2 = "./test_search_files/project2.txt";
const char *file3 = "./test_search_files/random.txt";

int CreateDir(const char *dir)
{
  return mkdir(dir, 0777) == 0;
}

int RemoveDir(const char *dir)
{
  if (rmdir(dir) == 0)
  {
    return 1;
  }
  else
  {
    perror("rmdir failed");
    return 0;
  }
}

Test(FileUtilsTest, SearchFiles_Success)
{
  cr_assert(CreateDir(testDir), "Failed to create directory");
  cr_assert(CreateFile(file1), "Failed to create file1");
  cr_assert(CreateFile(file2), "Failed to create file2");
  cr_assert(CreateFile(file3), "Failed to create file3");

  size_t count;
  FileInfo *results = SearchFiles(testDir, "project", &count);
  cr_assert(results != NULL, "SearchFiles returned NULL");

  cr_assert_eq(count, 2, "Expected 2 results");
  cr_assert_str_eq(results[0].name, "project1.txt");
  cr_assert_str_eq(results[1].name, "project2.txt");

  Cleanup(file1);
  Cleanup(file2);
  Cleanup(file3);
  RemoveDir(testDir);
}

Test(FileUtilsTest, SearchFiles_Failure)
{
  cr_assert(CreateDir(testDir), "Failed to create directory");
  cr_assert(CreateFile(file1), "Failed to create file1");
  cr_assert(CreateFile(file2), "Failed to create file2");
  cr_assert(CreateFile(file3), "Failed to create file3");

  size_t count;
  FileInfo *results = SearchFiles(testDir, "non_existent", &count);
  cr_assert(results != NULL, "SearchFiles returned NULL");
  cr_assert_eq(count, 0, "Expected empty results");

  Cleanup(file1);
  Cleanup(file2);
  Cleanup(file3);
  RemoveDir(testDir);
}
