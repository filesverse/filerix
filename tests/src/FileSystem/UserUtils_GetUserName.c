#include "filerix/FileSystem/UserUtils.h"

#include <criterion/criterion.h>

Test(UserUtils, GetUserName_Success)
{
  const char *username = GetUserName();
  cr_assert_not_null(username, "Username should not be NULL");
  cr_assert(strlen(username) > 0, "Username should not be empty");
}

Test(UserUtils, GetUserName_Failure)
{
  const char *invalidUsername = GetUserName();
  cr_assert_not_null(invalidUsername, "Even an invalid user should return a non-null value");
}
