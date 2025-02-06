#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <unistd.h>

Test(FileUtilsTest, Copy_Success)
{
  cr_assert(CreateFile("./test_file.txt"), "File creation failed");

  bool result = Copy("./test_file.txt", "./file_copy.txt");
  cr_assert(result, "File copy failed");

  cr_assert(FileExists("./file_copy.txt"), "Copied file does not exist");

  Cleanup("./test_file.txt");
  Cleanup("./file_copy.txt");
}

Test(FileUtilsTest, Copy_Failure, .init = SetupRedirect, .exit_code = EXIT_FAILURE)
{
  Copy("./non_existent.txt", "./file_copy.txt");
}
