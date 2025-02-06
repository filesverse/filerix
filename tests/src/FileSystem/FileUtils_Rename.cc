#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"

const std::string originalPath = "./file.txt";
const std::string renamedPath = "./file_renamed.txt";
const std::string invalidPath = "./invalid.txt";

Test(FileUtilsTest, RenameTest_Success)
{
  cr_assert(CreateFile(originalPath.c_str()), "failed to create file.");

  cr_assert(FileUtils::Rename(originalPath, renamedPath), "failed to rename file");
  Cleanup(renamedPath.c_str());
}

Test(FileUtilsTest, RenameTest_Failure, .init = SetupRedirect, .exit_code = EXIT_FAILURE)
{
  FileUtils::Rename(invalidPath, renamedPath);
}
