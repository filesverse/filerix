#include <criterion/criterion.h>
#include "filerix/FileSystem/FileUtils.h"
#include "TestUtils.h"
#include <filesystem>

const std::string source = "./test_file_gz.txt";
const std::string destination = "./test_file.gz";
const std::string invalidSource = "./non_existent.txt";

Test(FileUtilsTest, Compress_Success)
{
  cr_assert(CreateFile(source.c_str()), "File creation failed");

  bool result = FileUtils::Compress(source, destination);
  cr_assert(result, "Compression failed");

  cr_assert(std::filesystem::exists(destination), "Compressed file does not exist");

  Cleanup(source.c_str());
  Cleanup(destination.c_str());
}

Test(FileUtilsTest, Compress_Failure, .init = setup_redirect, .exit_code = EXIT_FAILURE)
{
  FileUtils::Compress(invalidSource, destination);
}
