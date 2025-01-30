#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>

typedef enum
{
  NONE,
  ERROR,
  WARNING,
  INFO
} LogLevel;

static LogLevel currentLogLevel = NONE;

void createDirectories(const char *path)
{
  char dir[1024];
  snprintf(dir, sizeof(dir), "%s", path);
  char *p = dir + 1;
  while ((p = strchr(p, '/')) != NULL)
  {
    *p = '\0';
    mkdir(dir, 0755);
    *p = '/';
    p++;
  }
}

static void logToFile(const char *logType, const char *format, ...)
{
  const char *homeDir = getenv("HOME");
  if (!homeDir)
  {
    fprintf(stderr, "Error: Unable to find home directory.\n");
    return;
  }

  char logFilePath[1024];
  snprintf(logFilePath, sizeof(logFilePath), "%s/.local/libfm/%s_log.txt", homeDir, logType);

  createDirectories(logFilePath);

  FILE *logFile = fopen(logFilePath, "a");
  if (logFile != NULL)
  {
    va_list args;
    va_start(args, format);

    char messageBuffer[1024];
    vsnprintf(messageBuffer, sizeof(messageBuffer), format, args);
    va_end(args);

    time_t now = time(NULL);
    struct tm *timeInfo = localtime(&now);
    char timeBuffer[80];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);

    fprintf(logFile, "[%s] %s: %s\n", timeBuffer, logType, messageBuffer);
    fclose(logFile);
  }
  else
  {
    fprintf(stderr, "Error logging to file: %s\n", format);
  }
}

void Init(const char *logLevel)
{
  if (strcmp(logLevel, "error") == 0)
  {
    currentLogLevel = ERROR;
  }
  else if (strcmp(logLevel, "info") == 0 || strcmp(logLevel, "debug") == 0)
  {
    currentLogLevel = INFO;
  }
  else
  {
    currentLogLevel = NONE;
  }
}

void logger_error(const char *format, ...)
{
  if (currentLogLevel == ERROR || currentLogLevel == WARNING || currentLogLevel == INFO)
  {
    va_list args;
    va_start(args, format);
    logToFile("ERROR", format, args);
    va_end(args);
  }
}

void logger_warning(const char *format, ...)
{
  if (currentLogLevel == WARNING || currentLogLevel == INFO)
  {
    va_list args;
    va_start(args, format);
    logToFile("WARNING", format, args);
    va_end(args);
  }
}

void logger_info(const char *format, ...)
{
  if (currentLogLevel == INFO)
  {
    va_list args;
    va_start(args, format);
    logToFile("INFO", format, args);
    va_end(args);
  }
}
