#ifndef FILETYPES_H
#define FILETYPES_H

#include <stddef.h>

typedef struct
{
  const char *extension;
  const char *mime_type;
} MimeType;

static const MimeType MIME_TYPES[] = {
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".bmp", "image/bmp"},
    {".tiff", "image/tiff"},
    {".webp", "image/webp"},
    {".svg", "image/svg+xml"},

    {".mp3", "audio/mpeg"},
    {".wav", "audio/wav"},
    {".ogg", "audio/ogg"},
    {".flac", "audio/flac"},

    {".mp4", "video/mp4"},
    {".webm", "video/webm"},
    {".avi", "video/x-msvideo"},

    {".txt", "text/plain"},
    {".csv", "text/csv"},
    {".json", "application/json"}};

#endif
