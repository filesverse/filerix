#ifndef NAPI_BINDINGS_HPP
#define NAPI_BINDINGS_HPP

#include <napi.h>
#include "bindings/FilesBinding.hpp"
#include "bindings/DrivesBinding.hpp"
#include "bindings/UserBinding.hpp"
#include "bindings/CompressionBinding.hpp"
#include "bindings/LoggerBinding.hpp"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set("getFiles", Napi::Function::New(env, libfm::GetFiles));
  exports.Set("searchFiles", Napi::Function::New(env, libfm::SearchFiles));
  exports.Set("copyFile", Napi::Function::New(env, libfm::CopyFile));
  exports.Set("cutFile", Napi::Function::New(env, libfm::CutFile));
  exports.Set("renameFile", Napi::Function::New(env, libfm::RenameFile));

  exports.Set("getDriveUsage", Napi::Function::New(env, libfm::GetDriveUsage));
  exports.Set("getDrives", Napi::Function::New(env, libfm::GetDrives));
  exports.Set("mountDrive", Napi::Function::New(env, libfm::MountDrive));
  exports.Set("unmountDrive", Napi::Function::New(env, libfm::UnmountDrive));
  exports.Set("getDeviceLabelOrUUID", Napi::Function::New(env, libfm::GetDeviceLabelOrUUID));

  exports.Set("getUserName", Napi::Function::New(env, libfm::GetUserName));
  exports.Set("changePermissions", Napi::Function::New(env, libfm::ChangePermissions));

  exports.Set("compressFile", Napi::Function::New(env, libfm::CompressFile));
  exports.Set("decompressFile", Napi::Function::New(env, libfm::DecompressFile));

  exports.Set("initLogger", Napi::Function::New(env, libfm::InitLogger));

  return exports;
}

#endif
