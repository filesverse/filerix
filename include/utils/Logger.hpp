#ifndef ERROR_LOGGING_HPP
#define ERROR_LOGGING_HPP

#include <string>

namespace Logger
{
  void Error(const std::string &message);
  void Info(const std::string &message);
}

#endif
