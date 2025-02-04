#ifndef DRIVES_BINDING_HPP
#define DRIVES_BINDING_HPP

#include "filerix/FileSystem/DriveUtils.h"

#include <napi.h>

namespace filerix
{
  inline Napi::Array GetDrives(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    auto drives = DriveUtils::GetDrives();
    Napi::Array driveArray = Napi::Array::New(env, drives.size());

    for (size_t i = 0; i < drives.size(); ++i)
    {
      Napi::Object driveObj = Napi::Object::New(env);
      driveObj.Set("device", Napi::String::New(env, drives[i].device));
      driveObj.Set("status", Napi::String::New(env, drives[i].status));
      driveObj.Set("unmountable", Napi::Boolean::New(env, drives[i].unmountable));
      driveObj.Set("mountPoint", Napi::String::New(env, drives[i].mountPoint));
      driveObj.Set("partition", Napi::String::New(env, drives[i].partition));
      driveObj.Set("fsType", Napi::String::New(env, drives[i].fsType));

      driveArray[i] = driveObj;
    }

    return driveArray;
  }

  inline Napi::Object GetDriveUsage(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString())
    {
      Napi::TypeError::New(env, "Drive path must be a string").ThrowAsJavaScriptException();
      return Napi::Object::New(env);
    }

    std::string drive = info[0].As<Napi::String>().Utf8Value();
    auto usage = DriveUtils::GetDriveUsage(drive);

    Napi::Object result = Napi::Object::New(env);
    result.Set("used_space", Napi::Number::New(env, usage.used));
    result.Set("total_space", Napi::Number::New(env, usage.total));

    return result;
  }

  inline Napi::Boolean MountDrive(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
      Napi::TypeError::New(env, "Source must be a string").ThrowAsJavaScriptException();
      return Napi::Boolean::New(env, false);
    }

    std::string source = info[0].As<Napi::String>().Utf8Value();
    return Napi::Boolean::New(env, DriveUtils::MountDrive(source));
  }

  inline Napi::Boolean UnmountDrive(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString())
    {
      Napi::TypeError::New(env, "Target must be a string").ThrowAsJavaScriptException();
      return Napi::Boolean::New(env, false);
    }

    std::string target = info[0].As<Napi::String>().Utf8Value();
    return Napi::Boolean::New(env, DriveUtils::UnmountDrive(target));
  }

  inline Napi::String GetDeviceLabelOrUUID(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString())
    {
      Napi::TypeError::New(env, "Device path must be a string").ThrowAsJavaScriptException();
      return Napi::String::New(env, "");
    }

    std::string device = info[0].As<Napi::String>().Utf8Value();
    std::string labelOrUUID = DriveUtils::GetDeviceLabelOrUUID(device);

    return Napi::String::New(env, labelOrUUID);
  }
}

#endif
