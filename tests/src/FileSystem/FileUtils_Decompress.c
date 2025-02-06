#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <unistd.h>

Test(FileUtilsTest, Decompress_Success)
{
  cr_assert(CreateFile("./test_file.gz"), "File creation failed");

  bool result = Decompress("./test_file.gz", "./test_file_gz.txt");
  cr_assert(result, "Decompression failed");

  cr_assert(FileExists("./test_file_gz.txt"), "Decompressed file does not exist");

  Cleanup("./test_file.gz");
  Cleanup("./test_file_gz.txt");
}

Test(FileUtilsTest, Decompress_Failure, .init = SetupRedirect, .exit_code = EXIT_FAILURE)
{
  Decompress("./non_existent.gz", "./test_file_gz.txt");
}
