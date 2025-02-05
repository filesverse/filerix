#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <filesystem>

Test(FileUtilsTest, Cut_Success)
{
  std::string source = "./test_file.txt";
  std::string destination = "./file_moved.txt";

  cr_assert(CreateFile(source.c_str()), "File creation failed");

  bool result = FileUtils::Cut(source, destination);
  cr_assert(result, "File move (cut) failed");

  cr_assert(FileExists(destination.c_str()), "Moved file does not exist");
  cr_assert_not(FileExists(source.c_str()), "Source file should no longer exist");

  Cleanup(destination.c_str());
}

Test(FileUtilsTest, Cut_Failure, .init = setup_redirect, .exit_code = EXIT_FAILURE)
{
  std::string invalidSource = "./non_existent.txt";
  std::string destination = "./file_moved.txt";

  FileUtils::Cut(invalidSource, destination);
}
