#ifndef NAPI_BINDINGS_HPP
#define NAPI_BINDINGS_HPP

#include <napi.h>
#include "utils/FileUtils.hpp"
#include "FileSystem/FileUtils.hpp"
#include "FileSystem/MountUtils.hpp"
#include "FileSystem/UserUtils.hpp"

namespace fmlib
{
  Napi::Array GetFiles(const Napi::CallbackInfo &info)
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

  Napi::Array SearchFiles(const Napi::CallbackInfo &info)
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

  Napi::String GetUserName(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    return Napi::String::New(env, UserUtils::GetUserName());
  }

  Napi::Array GetMountedDrives(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    auto drives = MountUtils::GetMountedDrives();

    Napi::Array result = Napi::Array::New(env, drives.size());
    for (size_t i = 0; i < drives.size(); ++i)
    {
      result[i] = Napi::String::New(env, drives[i]);
    }

    return result;
  }

  Napi::Object GetDriveUsage(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString())
    {
      Napi::TypeError::New(env, "Drive path must be a string").ThrowAsJavaScriptException();
      return Napi::Object::New(env);
    }

    std::string drive = info[0].As<Napi::String>().Utf8Value();
    auto usage = MountUtils::GetDriveUsage(drive);

    Napi::Object result = Napi::Object::New(env);
    result.Set("used_space", usage.first);
    result.Set("total_space", usage.second);

    return result;
  }

}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set("getFiles", Napi::Function::New(env, fmlib::GetFiles));
  exports.Set("getUserName", Napi::Function::New(env, fmlib::GetUserName));
  exports.Set("getMountedDrives", Napi::Function::New(env, fmlib::GetMountedDrives));
  exports.Set("getDriveUsage", Napi::Function::New(env, fmlib::GetDriveUsage));
  return exports;
}

#endif
