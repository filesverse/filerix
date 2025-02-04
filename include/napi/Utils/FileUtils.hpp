#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include "filerix/FileSystem/FileInfo.h"

#include <napi.h>
#include <vector>

namespace FileUtils
{
  Napi::Object CreateFileObject(Napi::Env env, const FileInfo &file)
  {
    Napi::Object fileObj = Napi::Object::New(env);
    fileObj.Set("name", file.name);
    fileObj.Set("type", file.type);
    fileObj.Set("path", file.path);
    fileObj.Set("size", file.size);
    return fileObj;
  }

  Napi::Array CreateFileArray(Napi::Env env, const std::vector<FileInfo> &files)
  {
    Napi::Array result = Napi::Array::New(env, files.size());
    for (size_t i = 0; i < files.size(); ++i)
    {
      result[i] = CreateFileObject(env, files[i]);
    }
    return result;
  }
}

#endif