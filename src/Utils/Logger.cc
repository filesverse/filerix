#include "Utils/Logger.h"

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <filesystem>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <sstream>

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
  static std::vector<std::string> logPrefixes;

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

  static std::vector<std::string> splitString(const std::string &str, char delimiter)
  {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
      if (!token.empty())
        tokens.push_back(token);
    }

    return tokens;
  }

  static void initLoggerFromEnv()
  {
    const char *logLevelEnv = std::getenv("filerix_log_level");
    const char *logPrefixEnv = std::getenv("filerix_log");

    if (logLevelEnv)
      currentLogLevel = getLogLevelFromString(logLevelEnv);
    else
      currentLogLevel = LogLevel::NONE;

    if (logPrefixEnv)
      logPrefixes = splitString(logPrefixEnv, ',');
    else
      logPrefixes.clear();
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

  static bool shouldLog(const std::string &message)
  {
    if (logPrefixes.empty())
      return true;

    for (const std::string &prefix : logPrefixes)
    {
      if (message.find("[" + prefix + "]") == 0)
        return true;
    }

    return false;
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

    if (!shouldLog(message))
      return;

    if (currentLogLevel == LogLevel::DEBUG)
    {
      std::cout << "\033[1;32m[ filerix ]\033[0m \033[1;34m[ INFO ]\033[0m " << message << std::endl;

      logToFile("INFO", message);
    }
  }
}
