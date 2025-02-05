#include <criterion/criterion.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"

Test(FileUtilsTest, Compress_Success, .init = setup_redirect)
{
  cr_assert(CreateFile("./test_file_gz.txt"), "File creation failed");

  bool result = Compress("./test_file_gz.txt", "./test_file.gz");
  cr_assert(result, "Compression failed");

  cr_assert(FileExists("./test_file.gz"), "Compressed file does not exist");

  Cleanup("./test_file_gz.txt");
  Cleanup("./test_file.gz");
}

Test(FileUtilsTest, Compress_Failure, .init = setup_redirect, .exit_code = EXIT_FAILURE)
{
  Compress("./non_existent.txt", "./test_file.gz");
}
