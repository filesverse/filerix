# **User**

## getUserName()
- **`getUserName(): string`**  
  - **Description:** Retrieves the username of the currently logged-in user.  
  - **Returns:**  
    - The username as a string.  
  - **Examples:**

:::code-group

```cpp [<i class="devicon-cplusplus-plain colored"></i> C++]
#include <iostream>
#include "filerix/FileSystem/UserUtils.h"

int main() {
  const char *username = GetUserName();

  if (username) {
    std::cout << "Username: ", << username << std::endl;
  } else {
    std::cerr << "Failed to get username." << std::endl;
  }

  return 0;
}
```

```c [<i class="devicon-c-plain colored"></i> C]
#include <stdio.h>
#include "filerix/FileSystem/UserUtils.h"

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

```javascript [<i class="devicon-nodejs-plain colored"></i> Node.js]
import { getUserName } from "@kingmaj0r/filerix/lib";

console.log(`Current user: ${filerix.getUserName()}`);
```

```rust [<i class="devicon-rust-plain colored" style="color:#CE422B;"></i> Rust]
use filerix::FileSystem::UserUtils;

fn main() {
  match UserUtils::get_user_name() {
    Some(username) => println!("Username: {}", username),
    None => eprintln!("Failed to get username."),
  }
}
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

```cpp [<i class="devicon-cplusplus-plain colored"></i> C++]
#include <iostream>
#include "filerix/FileSystem/UserUtils.h"

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

```c [<i class="devicon-c-plain colored"></i> C]
#include <stdio.h>
#include "filerix/FileSystem/UserUtils.h"

int main() {
  const char* filePath = "/path/to/file";
  int mode = 0644;

  if (ChangePermissions(filePath, mode)) {
    printf("Successfully changed permissions.\n");
  } else {
    fprintf(stderr, "Failed to set permissions.\n");
  }

  return 0;
}
```

```javascript [<i class="devicon-nodejs-plain colored"></i> Node.js]
import { changePermissions } from "@kingmaj0r/filerix/lib";

if (changePermissions('/path/to/file', 0o644)) {
    console.log('Permissions updated successfully.');
} else {
    console.log('Failed to update permissions.');
}
```

```rust [<i class="devicon-rust-plain colored" style="color:#CE422B;"></i> Rust]
use filerix::FileSystem::UserUtils;

fn main() {
  let file_path = "/path/to/file";
  let mode = 0o644;

  if UserUtils::change_permissions(file_path, mode) {
    println!("Permissions updated successfully.");
  } else {
    eprintln!("Failed to update permissions.");
  }
}
```

:::
