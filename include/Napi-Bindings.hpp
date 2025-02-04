#ifndef NAPI_BINDINGS_HPP
#define NAPI_BINDINGS_HPP

#include <napi.h>
#include "napi/Bindings/FileSystem/FilesBinding.hpp"
#include "napi/Bindings/FileSystem/DrivesBinding.hpp"
#include "napi/Bindings/FileSystem/UserBinding.hpp"
#include "napi/Bindings/FileSystem/CompressionBinding.hpp"
#include "napi/Bindings/Listeners/DriveListenerBinding.hpp"
#include "napi/Bindings/Listeners/FileListenerBinding.hpp"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set("getFiles", Napi::Function::New(env, filerix::GetFiles));
  exports.Set("searchFiles", Napi::Function::New(env, filerix::SearchFiles));
  exports.Set("copyFile", Napi::Function::New(env, filerix::CopyFile));
  exports.Set("cutFile", Napi::Function::New(env, filerix::CutFile));
  exports.Set("renameFile", Napi::Function::New(env, filerix::RenameFile));

  exports.Set("getDriveUsage", Napi::Function::New(env, filerix::GetDriveUsage));
  exports.Set("getDrives", Napi::Function::New(env, filerix::GetDrives));
  exports.Set("mountDrive", Napi::Function::New(env, filerix::MountDrive));
  exports.Set("unmountDrive", Napi::Function::New(env, filerix::UnmountDrive));
  exports.Set("getDeviceLabelOrUUID", Napi::Function::New(env, filerix::GetDeviceLabelOrUUID));

  exports.Set("getUserName", Napi::Function::New(env, filerix::GetUserName));
  exports.Set("changePermissions", Napi::Function::New(env, filerix::ChangePermissions));

  exports.Set("compressFile", Napi::Function::New(env, filerix::CompressFile));
  exports.Set("decompressFile", Napi::Function::New(env, filerix::DecompressFile));

  exports.Set("startDriveListener", Napi::Function::New(env, filerix::StartDriveListener));
  exports.Set("stopDriveListener", Napi::Function::New(env, filerix::StopDriveListener));
  exports.Set("startFileListener", Napi::Function::New(env, filerix::StartFileListener));
  exports.Set("stopFileListener", Napi::Function::New(env, filerix::StopFileListener));

  return exports;
}

#endif
