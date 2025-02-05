#include "include/Utils/Logger.h"

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <filesystem>
#include <unistd.h>
#include <cstring>

namespace Logger
{
  enum class LogLevel
  {
    NONE,
    ERROR,
    WARNING,
    DEBUG
  };

  static LogLevel currentLogLevel = LogLevel::NONE;

  static LogLevel getLogLevelFromString(const char *logLevel)
  {
    if (strcmp(logLevel, "error") == 0)
      return LogLevel::ERROR;
    else if (strcmp(logLevel, "warning") == 0)
      return LogLevel::WARNING;
    else if (strcmp(logLevel, "debug") == 0)
      return LogLevel::DEBUG;
    return LogLevel::NONE;
  }

  static void initLoggerFromEnv()
  {
    const char *logLevelEnv = std::getenv("filerix_log_level");

    if (logLevelEnv)
    {
      currentLogLevel = getLogLevelFromString(logLevelEnv);
    }
    else
    {
      currentLogLevel = LogLevel::NONE;
    }
  }

  static void logToFile(const std::string &logType, const std::string &message)
  {
    const char *homeDir = std::getenv("HOME");
    if (!homeDir)
    {
      std::cerr << "Error: Unable to find home directory." << std::endl;
      return;
    }

    std::filesystem::path logFilePath = std::filesystem::path(homeDir) / ".local" / "filerix" / (logType + "_log.txt");

    std::filesystem::create_directories(logFilePath.parent_path());

    std::ofstream logFile(logFilePath, std::ios::app);
    if (logFile.is_open())
    {
      std::time_t now = std::time(nullptr);
      std::tm *timeInfo = std::localtime(&now);
      char timeBuffer[80];
      std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);

      logFile << "[" << timeBuffer << "] " << logType << ": " << message << std::endl;
    }
    else
    {
      std::cerr << "Error logging to file: " << message << std::endl;
    }
  }

  void Error(const std::string &message)
  {
    initLoggerFromEnv();

    std::cerr << "\033[1;32m[ filerix ]\033[0m \033[1;31m[ ERROR ]\033[0m " << message << std::endl;

    if (currentLogLevel == LogLevel::ERROR || currentLogLevel == LogLevel::WARNING || currentLogLevel == LogLevel::DEBUG)
    {
      logToFile("ERROR", message);
    }

    exit(EXIT_FAILURE);
  }

  void Warn(const std::string &message)
  {
    initLoggerFromEnv();

    if (currentLogLevel == LogLevel::WARNING || currentLogLevel == LogLevel::DEBUG)
    {
      std::cerr << "\033[1;32m[ filerix ]\033[0m \033[1;33m[ WARN ]\033[0m " << message << std::endl;

      logToFile("WARNING", message);
    }
  }

  void Info(const std::string &message)
  {
    initLoggerFromEnv();

    if (currentLogLevel == LogLevel::DEBUG)
    {
      std::cout << "\033[1;32m[ filerix ]\033[0m \033[1;34m[ INFO ]\033[0m " << message << std::endl;

      logToFile("INFO", message);
    }
  }
}
