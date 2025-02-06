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

Test(FileUtilsTest, SearchFiles_Success)
{
  cr_assert(CreateDir(testDir), "Failed to create directory");
  cr_assert(CreateFile(file1), "Failed to create file1");
  cr_assert(CreateFile(file2), "Failed to create file2");
  cr_assert(CreateFile(file3), "Failed to create file3");

  struct FileList results = SearchFiles(testDir, "project");
  cr_assert(results.files != NULL, "SearchFiles returned NULL");

  cr_assert_eq(results.count, 2, "Expected 2 results");
  cr_assert_str_eq(results.files[0].name, "project1.txt");
  cr_assert_str_eq(results.files[1].name, "project2.txt");

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

  struct FileList results = SearchFiles(testDir, "non_existent");
  cr_assert(results.files != NULL, "SearchFiles returned NULL");
  cr_assert_eq(results.count, 0, "Expected empty results");

  Cleanup(file1);
  Cleanup(file2);
  Cleanup(file3);
  RemoveDir(testDir);
}
