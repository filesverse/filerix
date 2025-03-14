#include "FileSystem/FileUtils.h"
#include "FileSystem/DirectoryPermissions.h"
#include "Utils/Logger.h"
#include "data/FileTypes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <zlib.h>
#include <ctype.h>

typedef struct
{
  char *name;
  char *type;
  char *path;
  size_t size;
  int isDirectory;
} FileInfo;

char *CopyString(const char *str)
{
  char *copy = (char *)malloc(strlen(str) + 1);
  if (copy)
    strcpy(copy, str);
  return copy;
}

char *GetMimeType(const char *filePath)
{
  Logger_Info("[FileUtils] Getting MIME type for: %s", filePath);

  struct stat path_stat;
  if (stat(filePath, &path_stat) != 0)
  {
    Logger_Error("[FileUtils] Error getting MIME type: stat failed");
    return CopyString("unknown");
  }

  if (S_ISDIR(path_stat.st_mode))
  {
    return CopyString("directory");
  }

  const char *ext = strrchr(filePath, '.');
  if (!ext || ext == filePath)
  {
    return CopyString("application/octet-stream");
  }

  for (size_t i = 0; i < sizeof(MIME_TYPES) / sizeof(MIME_TYPES[0]); i++)
  {
    if (strcmp(ext, MIME_TYPES[i].extension) == 0)
    {
      return CopyString(MIME_TYPES[i].mime_type);
    }
  }

  return CopyString("application/octet-stream");
}

struct FileList FileUtils_GetFiles(const char *path)
{
  Logger_Info("[FileUtils] Getting files from: %s", path);

  struct FileList fileList = {NULL, 0};
  DIR *dir = opendir(path);
  if (!dir)
  {
    Logger_Error("[FileUtils] Directory does not exist or cannot be opened");
    return fileList;
  }

  struct dirent *entry;
  struct FileInfo *files = NULL;
  size_t fileCount = 0;

  while ((entry = readdir(dir)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

    struct FileInfo fileInfo;
    fileInfo.name = CopyString(entry->d_name);
    fileInfo.type = GetMimeType(fullPath);
    fileInfo.path = CopyString(fullPath);

    struct stat st;
    stat(fullPath, &st);
    fileInfo.size = S_ISREG(st.st_mode) ? st.st_size : 0;
    fileInfo.isDirectory = S_ISDIR(st.st_mode);

    files = (struct FileInfo *)realloc(files, sizeof(struct FileInfo) * (fileCount + 1));
    files[fileCount++] = fileInfo;
  }

  closedir(dir);
  fileList.files = files;
  fileList.count = fileCount;
  return fileList;
}

bool FileUtils_Copy(const char *source, const char *destination)
{
  Logger_Info("[FileUtils] Copying from %s to %s", source, destination);

  int srcFd = open(source, O_RDONLY);
  if (srcFd < 0)
  {
    Logger_Error("[FileUtils] Failed to open source file: %s", source);
    return 0;
  }

  int destFd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (destFd < 0)
  {
    close(srcFd);
    Logger_Error("[FileUtils] Failed to create destination file: %s", destination);
    return 0;
  }

  char buffer[4096];
  ssize_t bytesRead;
  while ((bytesRead = read(srcFd, buffer, sizeof(buffer))) > 0)
  {
    write(destFd, buffer, bytesRead);
  }

  close(srcFd);
  close(destFd);
  return 1;
}

bool FileUtils_Rename(const char *oldPath, const char *newPath)
{
  Logger_Info("[FileUtils] Renaming %s to %s", oldPath, newPath);

  if (rename(oldPath, newPath) == 0)
  {
    return 1;
  }

  Logger_Error("[FileUtils] Rename failed: %s -> %s", oldPath, newPath);
  return 0;
}

bool FileUtils_MoveTo(const char *source, const char *destination)
{
  Logger_Info("[FileUtils] Moving from %s to %s", source, destination);

  return FileUtils_Rename(source, destination);
}

bool FileUtils_Compress(const char *source, const char *destination)
{
  Logger_Info("[FileUtils] Compressing %s to %s", source, destination);

  FILE *inFile = fopen(source, "rb");
  if (!inFile)
  {
    Logger_Error("[FileUtils] Failed to open source file: %s", source);
    return 0;
  }

  FILE *outFile = fopen(destination, "wb");
  if (!outFile)
  {
    fclose(inFile);
    Logger_Error("[FileUtils] Failed to create destination file: %s", destination);
    return 0;
  }

  z_stream strm = {0};
  if (deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
  {
    fclose(inFile);
    fclose(outFile);
    Logger_Error("[FileUtils] Failed to initialize zlib");
    return 0;
  }

  unsigned char inBuffer[1024], outBuffer[1024];
  int flush;
  do
  {
    strm.avail_in = fread(inBuffer, 1, sizeof(inBuffer), inFile);
    strm.next_in = inBuffer;
    flush = feof(inFile) ? Z_FINISH : Z_NO_FLUSH;

    do
    {
      strm.avail_out = sizeof(outBuffer);
      strm.next_out = outBuffer;
      deflate(&strm, flush);
      fwrite(outBuffer, 1, sizeof(outBuffer) - strm.avail_out, outFile);
    } while (strm.avail_out == 0);

  } while (flush != Z_FINISH);

  deflateEnd(&strm);
  fclose(inFile);
  fclose(outFile);
  return 1;
}

bool FileUtils_Decompress(const char *source, const char *destination)
{
  Logger_Info("[FileUtils] Decompressing %s to %s", source, destination);

  FILE *inFile = fopen(source, "rb");
  if (!inFile)
  {
    Logger_Error("[FileUtils] Failed to open compressed file: %s", source);
    return 0;
  }

  FILE *outFile = fopen(destination, "wb");
  if (!outFile)
  {
    fclose(inFile);
    Logger_Error("[FileUtils] Failed to create destination file: %s", destination);
    return 0;
  }

  fseek(inFile, 10, SEEK_SET);

  z_stream strm = {0};
  if (inflateInit2(&strm, MAX_WBITS + 16) != Z_OK)
  {
    fclose(inFile);
    fclose(outFile);
    Logger_Error("[FileUtils] Failed to initialize zlib");
    return 0;
  }

  unsigned char inBuffer[1024], outBuffer[1024];
  int ret;
  do
  {
    strm.avail_in = fread(inBuffer, 1, sizeof(inBuffer), inFile);
    if (ferror(inFile))
      break;
    strm.next_in = inBuffer;

    do
    {
      strm.avail_out = sizeof(outBuffer);
      strm.next_out = outBuffer;
      ret = inflate(&strm, Z_NO_FLUSH);
      if (ret == Z_STREAM_ERROR)
        break;
      fwrite(outBuffer, 1, sizeof(outBuffer) - strm.avail_out, outFile);
    } while (strm.avail_out == 0);

  } while (ret != Z_STREAM_END);

  inflateEnd(&strm);
  fclose(inFile);
  fclose(outFile);
  return 1;
}
