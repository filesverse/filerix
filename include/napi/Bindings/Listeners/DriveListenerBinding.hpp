#ifndef DRIVE_LISTENER_BINDING_HPP
#define DRIVE_LISTENER_BINDING_HPP

#include "filerix/Listeners/DriveListener.h"

#include <napi.h>

namespace filerix
{
  static DriveListener::DriveMonitor driveMonitor;
  static Napi::ThreadSafeFunction tsfn_drive;

  void StartDriveListener(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (!info[0].IsFunction())
    {
      Napi::TypeError::New(env, "Callback function required").ThrowAsJavaScriptException();
      return;
    }

    tsfn_drive = Napi::ThreadSafeFunction::New(
        env,
        info[0].As<Napi::Function>(),
        "DriveListenerCallback",
        0,
        1);

    driveMonitor.Start([](std::string action, std::string device)
                       {
            if (!tsfn_drive) return;
            tsfn_drive.NonBlockingCall([action, device](Napi::Env env, Napi::Function jsCallback) {
                jsCallback.Call({Napi::String::New(env, action), Napi::String::New(env, device)});
            }); });
  }

  void StopDriveListener(const Napi::CallbackInfo &info)
  {
    driveMonitor.Stop();
    if (tsfn_drive)
    {
      tsfn_drive.Release();
    }
  }
}

#endif
