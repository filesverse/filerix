#ifndef FILES_BINDING_HPP
#define FILES_BINDING_HPP

#include "filerix/FileSystem/FileUtils.h"
#include "filerix/napi/Utils/FileUtils.hpp"

#include <napi.h>

namespace filerix
{
  inline Napi::Array GetFiles(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString())
    {
      Napi::TypeError::New(env, "Path must be a string").ThrowAsJavaScriptException();
      return Napi::Array::New(env);
    }

    std::string path = info[0].As<Napi::String>().Utf8Value();
    auto files = FileUtils::GetFiles(path);

    return FileUtils::CreateFileArray(env, files);
  }

  inline Napi::Array SearchFiles(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString())
    {
      Napi::TypeError::New(env, "Both path and query must be strings").ThrowAsJavaScriptException();
      return Napi::Array::New(env);
    }

    std::string path = info[0].As<Napi::String>().Utf8Value();
    std::string query = info[1].As<Napi::String>().Utf8Value();
    auto files = FileUtils::SearchFiles(path, query);

    return FileUtils::CreateFileArray(env, files);
  }

  inline Napi::Boolean CopyFile(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString())
    {
      Napi::TypeError::New(env, "Source and destination must be strings").ThrowAsJavaScriptException();
      return Napi::Boolean::New(env, false);
    }

    std::string source = info[0].As<Napi::String>().Utf8Value();
    std::string destination = info[1].As<Napi::String>().Utf8Value();

    return Napi::Boolean::New(env, FileUtils::Copy(source, destination));
  }

  inline Napi::Boolean CutFile(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString())
    {
      Napi::TypeError::New(env, "Source and destination must be strings").ThrowAsJavaScriptException();
      return Napi::Boolean::New(env, false);
    }

    std::string source = info[0].As<Napi::String>().Utf8Value();
    std::string destination = info[1].As<Napi::String>().Utf8Value();

    return Napi::Boolean::New(env, FileUtils::Cut(source, destination));
  }

  inline Napi::Boolean RenameFile(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString())
    {
      Napi::TypeError::New(env, "Old and new paths must be strings").ThrowAsJavaScriptException();
      return Napi::Boolean::New(env, false);
    }

    std::string oldPath = info[0].As<Napi::String>().Utf8Value();
    std::string newPath = info[1].As<Napi::String>().Utf8Value();

    return Napi::Boolean::New(env, FileUtils::Rename(oldPath, newPath));
  }
} // namespace fmlib

#endif
