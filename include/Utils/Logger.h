#ifndef ERROR_LOGGING_H
#define ERROR_LOGGING_H

#include <string>

namespace Logger
{
  void Error(const std::string &message);
  void Warn(const std::string &message);
  void Info(const std::string &message);
}

#endif
