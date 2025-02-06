#include <criterion/redirect.h>
#include "TestUtils.h"
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>

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

void SetupRedirect()
{
  cr_redirect_stdout();
  cr_redirect_stderr();
}

int RemoveDir(const std::string &path)
{
  std::string command = "rm -rf " + path;

  int result = system(command.c_str());

  if (result == -1)
  {
    std::cerr << "Error running system command: " << strerror(errno) << std::endl;
    return -1;
  }

  return 0;
}

int CreateDir(const std::string &path)
{
  return mkdir(path.c_str(), 0777) == 0;
}