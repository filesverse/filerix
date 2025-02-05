#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <filesystem>

Test(FileUtilsTest, Copy_Success)
{
  std::string source = "./test_file.txt";
  std::string destination = "./file_copy.txt";

  cr_assert(CreateFile(source.c_str()), "File creation failed");

  bool result = FileUtils::Copy(source, destination);
  cr_assert(result, "File copy failed");

  cr_assert(FileExists(destination.c_str()), "Copied file does not exist");

  Cleanup(source.c_str());
  Cleanup(destination.c_str());
}

Test(FileUtilsTest, Copy_Failure, .init = setup_redirect, .exit_code = EXIT_FAILURE)
{
  std::string invalidSource = "./non_existent.txt";
  std::string destination = "./file_copy.txt";

  FileUtils::Copy(invalidSource, destination);
}
