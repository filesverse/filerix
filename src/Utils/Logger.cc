#include "include/utils/Logger.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <filesystem>
#include <unistd.h>

namespace Logger
{
  void Error(const std::string &message)
  {
    const char *homeDir = std::getenv("HOME");
    if (!homeDir)
    {
      std::cerr << "Error: Unable to find home directory." << std::endl;
      return;
    }

    std::filesystem::path logFilePath = std::filesystem::path(homeDir) / ".local" / "libfm" / "error_log.txt";

    std::filesystem::create_directories(logFilePath.parent_path());

    std::ofstream logFile(logFilePath, std::ios::app);
    if (logFile.is_open())
    {
      std::time_t now = std::time(nullptr);
      std::tm *timeInfo = std::localtime(&now);
      char timeBuffer[80];
      std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);

      logFile << "[" << timeBuffer << "] ERROR: " << message << std::endl;
    }
    else
    {
      std::cerr << "Error logging to file: " << message << std::endl;
    }
  }

  void Info(const std::string &message)
  {
    const char *homeDir = std::getenv("HOME");
    if (!homeDir)
    {
      std::cerr << "Error: Unable to find home directory." << std::endl;
      return;
    }

    std::filesystem::path logFilePath = std::filesystem::path(homeDir) / ".local" / "libfm" / "info_log.txt";

    std::filesystem::create_directories(logFilePath.parent_path());

    std::ofstream logFile(logFilePath, std::ios::app);
    if (logFile.is_open())
    {
      std::time_t now = std::time(nullptr);
      std::tm *timeInfo = std::localtime(&now);
      char timeBuffer[80];
      std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);

      logFile << "[" << timeBuffer << "] INFO: " << message << std::endl;
    }
    else
    {
      std::cerr << "Error logging to file: " << message << std::endl;
    }
  }
}
