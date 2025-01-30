# **User**

## getUserName()
- **`getUserName(): string`**  
  - **Description:** Retrieves the username of the currently logged-in user.  
  - **Returns:**  
    - The username as a string.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/UserUtils.hpp"

int main() {
    try {
        std::string username = UserUtils::GetUserName();
        std::cout << "Current user: " << username << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

```c [C]
#include <stdio.h>
#include "libfm/FileSystem/UserUtils.h"

int main() {
  const char *username = GetUserName();

  if (username) {
    printf("Username: %s\n", username);
  } else {
    fprintf(stderr, "Failed to get username.\n");
  }

  return 0;
}
```

```javascript [Node.js]
import { getUserName } from "@kingmaj0r/libfm/lib";

console.log(`Current user: ${libfm.getUserName()}`);
```

:::

## changePermissions()
- **`changePermissions(path: string, mode: number): boolean`**  
  - **Description:** Changes file or directory permissions.  
  - **Parameters:**  
    - `path` (string): The file or directory path.  
    - `mode` (number): The permission mode (e.g., `0755`).  
  - **Returns:**  
    - `true` if successful, otherwise `false`.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/UserUtils.hpp"

int main() {
    const std::string filePath = "/path/to/file";
    int mode = 0644;

    if (UserUtils::ChangePermissions(filePath, mode)) {
        std::cout << "Permissions updated successfully." << std::endl;
    } else {
        std::cerr << "Failed to update permissions." << std::endl;
    }
    return 0;
}
```

```c [C]
#include <stdio.h>
#include <stdbool.h>
#include "libfm/FileSystem/UserUtils.h"

int main() {
  const char *username = GetUserName();

  char path[256];
  snprintf(path, sizeof(path), "/home/%s/%s", username, "test");

  bool success = ChangePermissions(path, 711);

  if (success) {
    printf("Successfully changed permissions.\n");
  } else {
    fprintf(stderr, "Failed to set permissions.\n");
  }

  return 0;
}
```

```javascript [Node.js]
import { changePermissions } from "@kingmaj0r/libfm/lib";

if (changePermissions('/path/to/file', 0o644)) {
    console.log('Permissions updated successfully.');
} else {
    console.log('Failed to update permissions.');
}
```

:::
