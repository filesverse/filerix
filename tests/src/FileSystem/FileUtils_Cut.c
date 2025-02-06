#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <unistd.h>

Test(FileUtilsTest, Cut_Success)
{
  cr_assert(CreateFile("./test_file.txt"), "File creation failed");

  bool result = Cut("./test_file.txt", "./file_moved.txt");
  cr_assert(result, "File move (cut) failed");

  cr_assert(FileExists("./file_moved.txt"), "Moved file does not exist");
  cr_assert_not(FileExists("./test_file.txt"), "Source file should no longer exist");

  Cleanup("./file_moved.txt");
}

Test(FileUtilsTest, Cut_Failure, .init = SetupRedirect, .exit_code = EXIT_FAILURE)
{
  Cut("./non_existent.txt", "./file_moved.txt");
}
