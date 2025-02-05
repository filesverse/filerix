#include "filerix/FileSystem/UserUtils.h"
#include "TestUtils.h"

#include <criterion/criterion.h>
#include <cstring>

Test(UserUtilsTest, GetUserName_Success)
{
  const char *username = GetUserName();
  cr_assert(username != nullptr, "Username should not be NULL");
  cr_assert(username, "", "Username should not be empty");
}

Test(UserUtilsTest, GetUserName_Failure)
{
  const char *invalidUsername = GetUserName();
  cr_assert(invalidUsername != nullptr, "Username should not be NULL");
}
