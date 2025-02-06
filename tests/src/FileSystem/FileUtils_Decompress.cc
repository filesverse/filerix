#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <filesystem>

Test(FileUtilsTest, Decompress_Success)
{
  std::string source = "./test_file.gz";
  std::string destination = "./test_file_gz.txt";

  cr_assert(CreateFile(source.c_str()), "File creation failed");

  bool result = FileUtils::Decompress(source, destination);
  cr_assert(result, "Decompression failed");

  cr_assert(FileExists(destination.c_str()), "Decompressed file does not exist");

  Cleanup(source.c_str());
  Cleanup(destination.c_str());
}

Test(FileUtilsTest, Decompress_Failure, .init = SetupRedirect, .exit_code = EXIT_FAILURE)
{
  std::string invalidSource = "./non_existent.gz";
  std::string destination = "./test_file_gz.txt";

  FileUtils::Decompress(invalidSource, destination);
}
