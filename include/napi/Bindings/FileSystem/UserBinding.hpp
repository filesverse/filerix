#ifndef USER_BINDING_HPP
#define USER_BINDING_HPP

#include "filerix/FileSystem/UserUtils.h"

#include <napi.h>

namespace filerix
{
  inline Napi::String GetUserName(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    return Napi::String::New(env, UserUtils::GetUserName());
  }

  inline Napi::Boolean ChangePermissions(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsNumber())
    {
      Napi::TypeError::New(env, "Path must be a string and mode a number").ThrowAsJavaScriptException();
      return Napi::Boolean::New(env, false);
    }

    std::string path = info[0].As<Napi::String>().Utf8Value();
    int mode = info[1].As<Napi::Number>().Int32Value();

    return Napi::Boolean::New(env, UserUtils::ChangePermissions(path, mode));
  }
}

#endif
