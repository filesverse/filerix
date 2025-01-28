#ifndef LOGGER_BINDING_HPP
#define LOGGER_BINDING_HPP

#include <napi.h>
#include "libfm/utils/Logger.hpp"

namespace libfm
{
  Napi::String InitLogger(const Napi::CallbackInfo &info)
  {
    if (info.Length() != 1 || !info[0].IsString())
    {
      Napi::TypeError::New(info.Env(), "Expected one string argument").ThrowAsJavaScriptException();
    }

    std::string logLevel = info[0].As<Napi::String>();

    Logger::Init(logLevel);

    return Napi::String::New(info.Env(), "Logger initialized with level: " + logLevel);
  }
}

#endif
