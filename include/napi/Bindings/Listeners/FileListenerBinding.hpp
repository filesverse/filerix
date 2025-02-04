#ifndef FILE_LISTENER_BINDING_HPP
#define FILE_LISTENER_BINDING_HPP

#include "filerix/Listeners/FileListener.h"

#include <napi.h>

namespace filerix
{
  static FileListener::FileMonitor *fileMonitor = nullptr;
  static Napi::ThreadSafeFunction tsfn_file;

  void StartFileListener(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (!info[0].IsString())
    {
      Napi::TypeError::New(env, "Directory path required as first argument").ThrowAsJavaScriptException();
      return;
    }

    if (!info[1].IsFunction())
    {
      Napi::TypeError::New(env, "Callback function required as second argument").ThrowAsJavaScriptException();
      return;
    }

    std::string directory = info[0].As<Napi::String>();

    if (fileMonitor)
    {
      fileMonitor->Stop();
      delete fileMonitor;
      fileMonitor = nullptr;
    }

    fileMonitor = new FileListener::FileMonitor(directory);

    tsfn_file = Napi::ThreadSafeFunction::New(
        env,
        info[1].As<Napi::Function>(),
        "FileListenerCallback",
        0,
        1);

    fileMonitor->Start([](std::string eventType, std::string filePath)
                       {
            if (!tsfn_file) return;
            tsfn_file.NonBlockingCall([eventType, filePath](Napi::Env env, Napi::Function jsCallback) {
                jsCallback.Call({Napi::String::New(env, eventType), Napi::String::New(env, filePath)});
            }); });
  }

  void StopFileListener(const Napi::CallbackInfo &info)
  {
    if (fileMonitor)
    {
      fileMonitor->Stop();
      delete fileMonitor;
      fileMonitor = nullptr;
    }

    if (tsfn_file)
    {
      tsfn_file.Release();
    }
  }
}

#endif
