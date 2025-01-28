#include "include/utils/Logger.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <filesystem>
#include <unistd.h>

namespace Logger
{
  enum class LogLevel
  {
    NONE,
    ERROR,
    WARNING,
    INFO
  };

  static LogLevel currentLogLevel = LogLevel::NONE;

  void Init(const std::string &logLevel)
  {
    if (logLevel == "error")
    {
      currentLogLevel = LogLevel::ERROR;
    }
    else if (logLevel == "info" || logLevel == "debug")
    {
      currentLogLevel = LogLevel::INFO;
    }
    else
    {
      currentLogLevel = LogLevel::NONE;
    }
  }

  void Error(const std::string &message)
  {
    if (currentLogLevel == LogLevel::ERROR || currentLogLevel == LogLevel::WARNING || currentLogLevel == LogLevel::INFO)
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
  }

  void Warn(const std::string &message)
  {
    if (currentLogLevel == LogLevel::WARNING || currentLogLevel == LogLevel::INFO)
    {
      const char *homeDir = std::getenv("HOME");
      if (!homeDir)
      {
        std::cerr << "Error: Unable to find home directory." << std::endl;
        return;
      }

      std::filesystem::path logFilePath = std::filesystem::path(homeDir) / ".local" / "libfm" / "warning_log.txt";

      std::filesystem::create_directories(logFilePath.parent_path());

      std::ofstream logFile(logFilePath, std::ios::app);
      if (logFile.is_open())
      {
        std::time_t now = std::time(nullptr);
        std::tm *timeInfo = std::localtime(&now);
        char timeBuffer[80];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);

        logFile << "[" << timeBuffer << "] WARNING: " << message << std::endl;
      }
      else
      {
        std::cerr << "Error logging to file: " << message << std::endl;
      }
    }
  }

  void Info(const std::string &message)
  {
    if (currentLogLevel == LogLevel::INFO)
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
}
