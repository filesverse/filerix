#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <filesystem>

bool CreateDirectory(const std::string &path)
{
  return std::filesystem::create_directories(path);
}

Test(FileUtilsTest, GetFiles_Success)
{
  std::string testDir = "./TestDir";
  std::string testFile1 = testDir + "/file1.txt";
  std::string testFile2 = testDir + "/file2.txt";

  cr_assert(CreateDirectory(testDir), "Failed to create test directory");
  cr_assert(CreateFile(testFile1.c_str()), "Failed to create test file1");
  cr_assert(CreateFile(testFile2.c_str()), "Failed to create test file2");

  auto files = FileUtils::GetFiles(testDir);

  cr_assert_eq(files.size(), 2, "Expected 2 files in directory");

  cr_assert_str_eq(files[0].name.c_str(), "file1.txt", "First file name mismatch");
  cr_assert_str_eq(files[1].name.c_str(), "file2.txt", "Second file name mismatch");

  Cleanup(testFile1.c_str());
  Cleanup(testFile2.c_str());
  std::filesystem::remove(testDir);
}

Test(FileUtilsTest, GetFiles_Failure, .init = setup_redirect, .exit_code = EXIT_FAILURE)
{
  std::string invalidDir = "./InvalidDir";

  auto files = FileUtils::GetFiles(invalidDir);

  cr_assert(files.empty(), "Expected empty result for invalid directory");
}
