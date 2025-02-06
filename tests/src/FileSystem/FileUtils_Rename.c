#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"

const char *originalPath = "./file.txt";
const char *renamedPath = "./file_renamed.txt";
const char *invalidPath = "./invalid.txt";

Test(FileUtilsTest, RenameTest_Success)
{
  cr_assert(CreateFile(originalPath), "Failed to create file");

  cr_assert(Rename(originalPath, renamedPath), "Failed to rename the file");
  Cleanup(renamedPath);
}

Test(FileUtilsTest, RenameTest_Failure, .init = SetupRedirect, .exit_code = EXIT_FAILURE)
{
  Rename(invalidPath, renamedPath);
}
