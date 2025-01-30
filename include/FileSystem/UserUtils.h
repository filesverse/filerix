#ifndef USERUTILS_H
#define USERUTILS_H

#include <sys/types.h>

char *GetUserName();
int ChangePermissions(const char *path, mode_t mode);

#endif
