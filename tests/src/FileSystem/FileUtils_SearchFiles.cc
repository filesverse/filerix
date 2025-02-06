#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <filesystem>

std::string testDir = "./test_search_files";
std::string file1 = testDir + "/project1.txt";
std::string file2 = testDir + "/project2.txt";
std::string file3 = testDir + "/random.txt";

Test(FileUtilsTest, SearchFiles_Success)
{
  std::filesystem::create_directories(testDir);
  CreateFile(file1.c_str());
  CreateFile(file2.c_str());
  CreateFile(file3.c_str());

  auto results = FileUtils::SearchFiles(testDir, "project");

  cr_assert_eq(results.size(), 2);
  cr_assert_str_eq(results[0].name, "project1.txt");
  cr_assert_str_eq(results[1].name, "project2.txt");

  Cleanup(file1.c_str());
  Cleanup(file2.c_str());
  Cleanup(file3.c_str());
  std::filesystem::remove_all(testDir);
}

Test(FileUtilsTest, SearchFiles_Failure)
{
  std::filesystem::create_directories(testDir);
  CreateFile(file1.c_str());
  CreateFile(file2.c_str());
  CreateFile(file3.c_str());

  auto results = FileUtils::SearchFiles(testDir, "non_existent");
  cr_assert(results.empty(), "Expected empty results");

  Cleanup(file1.c_str());
  Cleanup(file2.c_str());
  Cleanup(file3.c_str());
  std::filesystem::remove_all(testDir);
}
