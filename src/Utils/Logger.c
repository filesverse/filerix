#include "Utils/Logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef enum
{
  LOG_LEVEL_NONE,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_DEBUG
} LogLevel;

static LogLevel currentLogLevel = LOG_LEVEL_NONE;
static int consoleInitialized = 0; // Ensure initConsole() runs only once

#ifdef _WIN32
static HANDLE hConsole;
#endif

void loadLogLevel()
{
  char logLevelStr[64] = {0};

#ifdef _WIN32
  DWORD len = GetEnvironmentVariableA("FILERIX_LOG_LEVEL", logLevelStr, sizeof(logLevelStr));
  if (len == 0 || len >= sizeof(logLevelStr))
  {
    currentLogLevel = LOG_LEVEL_NONE;
    return;
  }
#else
  const char *envVar = getenv("FILERIX_LOG_LEVEL");
  if (!envVar)
  {
    currentLogLevel = LOG_LEVEL_NONE;
    return;
  }

  strncpy(logLevelStr, envVar, sizeof(logLevelStr) - 1);
  logLevelStr[sizeof(logLevelStr) - 1] = '\0';
#endif

  if (strcasecmp(logLevelStr, "error") == 0)
    currentLogLevel = LOG_LEVEL_ERROR;
  else if (strcasecmp(logLevelStr, "warning") == 0)
    currentLogLevel = LOG_LEVEL_WARNING;
  else if (strcasecmp(logLevelStr, "debug") == 0)
    currentLogLevel = LOG_LEVEL_DEBUG;
  else
    currentLogLevel = LOG_LEVEL_NONE;
}

void initConsole()
{
  if (consoleInitialized)
    return;

  loadLogLevel();
#ifdef _WIN32
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
  consoleInitialized = 1;
}

#ifdef _WIN32
void setConsoleColor(const char *color)
{
  struct
  {
    const char *name;
    WORD attr;
  } colors[] = {
      {"red", FOREGROUND_RED | FOREGROUND_INTENSITY},
      {"yellow", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY},
      {"blue", FOREGROUND_BLUE | FOREGROUND_INTENSITY},
      {"green", FOREGROUND_GREEN | FOREGROUND_INTENSITY},
      {"pink", FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY},
      {"purple", FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY},
      {"gold", FOREGROUND_RED | FOREGROUND_GREEN},
      {"bold", FOREGROUND_INTENSITY},
      {"underline", COMMON_LVB_UNDERSCORE}};

  for (size_t i = 0; i < sizeof(colors) / sizeof(colors[0]); i++)
  {
    if (strcmp(color, colors[i].name) == 0)
    {
      SetConsoleTextAttribute(hConsole, colors[i].attr);
      return;
    }
  }
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void resetConsoleColor()
{
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
#else
const char *getAnsiCode(const char *tag)
{
  if (strcmp(tag, "red") == 0)
    return "\033[1;31m";
  if (strcmp(tag, "yellow") == 0)
    return "\033[1;33m";
  if (strcmp(tag, "blue") == 0)
    return "\033[1;34m";
  if (strcmp(tag, "green") == 0)
    return "\033[1;32m";
  if (strcmp(tag, "pink") == 0 || strcmp(tag, "purple") == 0)
    return "\033[1;35m";
  if (strcmp(tag, "gold") == 0)
    return "\033[1;33m";
  if (strcmp(tag, "bold") == 0)
    return "\033[1m";
  if (strcmp(tag, "underline") == 0)
    return "\033[4m";
  if (strcmp(tag, "/bold") == 0 || strcmp(tag, "/underline") == 0 || strcmp(tag, "/gold") == 0)
    return "\033[0m";
  return "";
}
#endif

void processLogMessage(const char *message, char *output, size_t outputSize)
{
  const char *ptr = message;
  char *outPtr = output;
  size_t remaining = outputSize - 1;

  while (*ptr && remaining > 0)
  {
    if (*ptr == '[')
    {
      const char *end = strchr(ptr, ']');
      if (end)
      {
        char tag[32] = {0};
        size_t tagLen = end - ptr - 1;
        if (tagLen < sizeof(tag))
        {
          strncpy(tag, ptr + 1, tagLen);
          tag[tagLen] = '\0';
#ifdef _WIN32
          setConsoleColor(tag);
#else
          const char *ansiCode = getAnsiCode(tag);
          size_t codeLen = strlen(ansiCode);
          if (codeLen < remaining)
          {
            strcpy(outPtr, ansiCode);
            outPtr += codeLen;
            remaining -= codeLen;
          }
#endif
        }
        ptr = end + 1;
        continue;
      }
    }

    *outPtr++ = *ptr++;
    remaining--;
  }

  *outPtr = '\0';
}

void logMessage(const char *level, const char *color, const char *format, va_list args)
{
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), format, args);

  char formattedMessage[2048];
  processLogMessage(buffer, formattedMessage, sizeof(formattedMessage));

#ifdef _WIN32
  setConsoleColor("green");
  printf("[ filerix ] ");
  setConsoleColor(color);
  printf("[ %s ] ", level);
  resetConsoleColor();
  printf("%s\n", formattedMessage);
#else
  printf("\033[1;32m[ filerix ]\033[0m ");               // Green "filerix" label
  printf("%s[ %s ]\033[0m ", getAnsiCode(color), level); // Colorized log level
  printf("%s\n", formattedMessage);
#endif
}

void Logger_Error(const char *format, ...)
{
  initConsole();

  if (currentLogLevel >= LOG_LEVEL_ERROR)
  {
    va_list args;
    va_start(args, format);
    logMessage("ERROR", "red", format, args);
    va_end(args);
  }
}

void Logger_Warn(const char *format, ...)
{
  initConsole();

  if (currentLogLevel >= LOG_LEVEL_WARNING)
  {
    va_list args;
    va_start(args, format);
    logMessage("WARN", "yellow", format, args);
    va_end(args);
  }
}

void Logger_Info(const char *format, ...)
{
  initConsole();

  if (currentLogLevel >= LOG_LEVEL_DEBUG)
  {
    va_list args;
    va_start(args, format);
    logMessage("INFO", "blue", format, args);
    va_end(args);
  }
}
