#include <iostream>
#include <string>
#include "filerix/FileSystem/DriveUtils.h"

int main()
{
  std::string drivePath = "/";

  auto [usedBytes, totalBytes] = DriveUtils::GetDriveUsage(drivePath);

  std::cout << "Used space: " << usedBytes << " bytes" << std::endl;
  std::cout << "Total space: " << totalBytes << " bytes" << std::endl;

  return 0;
}
