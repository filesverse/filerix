#ifndef COMPRESSION_BINDING_HPP
#define COMPRESSION_BINDING_HPP

#include "filerix/FileSystem/FileUtils.h"

#include <napi.h>

namespace filerix
{
  inline Napi::Boolean CompressFile(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString())
    {
      Napi::TypeError::New(env, "Source and destination must be strings").ThrowAsJavaScriptException();
      return Napi::Boolean::New(env, false);
    }

    std::string source = info[0].As<Napi::String>().Utf8Value();
    std::string destination = info[1].As<Napi::String>().Utf8Value();
    return Napi::Boolean::New(env, FileUtils::Compress(source, destination));
  }

  inline Napi::Boolean DecompressFile(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString())
    {
      Napi::TypeError::New(env, "Source and destination must be strings").ThrowAsJavaScriptException();
      return Napi::Boolean::New(env, false);
    }

    std::string source = info[0].As<Napi::String>().Utf8Value();
    std::string destination = info[1].As<Napi::String>().Utf8Value();
    return Napi::Boolean::New(env, FileUtils::Decompress(source, destination));
  }
}

#endif
