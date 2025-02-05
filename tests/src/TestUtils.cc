#include <criterion/redirect.h>
#include "TestUtils.h"
#include <fstream>
#include <filesystem>
#include <unistd.h>

bool CreateFile(const char *path)
{
  std::ofstream file(path, std::ios::binary);
  if (!file)
    return false;

  file << "THIS IS A TEST!";

  return file.good();
}

bool Cleanup(const char *path)
{
  return std::filesystem::remove(path);
}

int FileExists(const char *path)
{
  return access(path, F_OK) != -1;
}

void setup_redirect()
{
  cr_redirect_stdout();
  cr_redirect_stderr();
}
