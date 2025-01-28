# **Filesystem**

## getFiles()
- **`getFiles(path: string): FileInfo[]`**  
  - **Description:** Lists all files and directories in the specified path.  
  - **Parameters:**  
    - `path` (string): The directory to list.  
  - **Returns:**  
    - An array of `FileInfo` objects:
      - `name` (string): File name.
      - `path` (string): Full file path.
      - `isDirectory` (boolean): Whether it is a directory.
      - `size` (number): File size (in bytes).
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/FileUtils.hpp"

int main() {
    auto files = FileUtils::GetFiles("/home/user/Documents");
    for (const auto& file : files) {
        std::cout << file.name << " - " 
                  << (file.isDirectory ? "Directory" : "File") << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { getFiles } from "@kingmaj0r/libfm/lib";

const files = getFiles('/home/user/Documents');
files.forEach(file => {
    console.log(`${file.name} - ${file.isDirectory ? 'Directory' : 'File'}`);
});
```

:::

## searchFiles()
- **`searchFiles(path: string, query: string): FileInfo[]`**  
  - **Description:** Searches for files matching the query within a directory (recursively).  
  - **Parameters:**  
    - `path` (string): The directory to search.  
    - `query` (string): The file name or pattern to search for.  
  - **Returns:**  
    - An array of `FileInfo` objects for matching files:
      - `name` (string): File name.
      - `path` (string): Full file path.
      - `isDirectory` (boolean): Whether it is a directory.
      - `size` (number): File size (in bytes).
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/FileUtils.hpp"

int main() {
    auto results = FileUtils::SearchFiles("/home/user/Documents", "project");
    for (const auto& file : results) {
        std::cout << file.name << " - " << file.path << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { searchFiles } from "@kingmaj0r/libfm/lib";

const results = searchFiles('/home/user/Documents', 'project');
results.forEach(file => {
    console.log(`${file.name} - ${file.path}`);
});
```

:::

## copyFile()
- **`copyFile(source: string, destination: string): boolean`**  
  - **Description:** Copies a file or directory to a specified location.  
  - **Parameters:**  
    - `source` (string): Path to the source file or directory.  
    - `destination` (string): Path to the destination file or directory.  
  - **Returns:**  
    - `true` if the operation was successful, otherwise `false`.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/FileUtils.hpp"

int main() {
    if (FileUtils::Copy("/home/user/file.txt", "/home/user/Documents/file_copy.txt")) {
        std::cout << "File copied successfully." << std::endl;
    } else {
        std::cerr << "Failed to copy file." << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { copyFile } from "@kingmaj0r/libfm/lib";

if (copyFile('/home/user/file.txt', '/home/user/Documents/file_copy.txt')) {
    console.log('File copied successfully.');
} else {
    console.log('Failed to copy file.');
}
```

:::

## cutFile()
- **`cutFile(source: string, destination: string): boolean`**  
  - **Description:** Moves a file or directory to a new location (cut and paste).  
  - **Parameters:**  
    - `source` (string): Path to the source file or directory.  
    - `destination` (string): Path to the destination file or directory.  
  - **Returns:**  
    - `true` if the operation was successful, otherwise `false`.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/FileUtils.hpp"

int main() {
    if (FileUtils::Cut("/home/user/file.txt", "/home/user/Documents/file_moved.txt")) {
        std::cout << "File moved successfully." << std::endl;
    } else {
        std::cerr << "Failed to move file." << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { cutFile } from "@kingmaj0r/libfm/lib";

if (cutFile('/home/user/file.txt', '/home/user/Documents/file_moved.txt')) {
    console.log('File moved successfully.');
} else {
    console.log('Failed to move file.');
}
```

:::

## renameFile()
- **`renameFile(oldPath: string, newPath: string): boolean`**  
  - **Description:** Renames or moves a file or directory.  
  - **Parameters:**  
    - `oldPath` (string): The current path of the file or directory.  
    - `newPath` (string): The new path of the file or directory.  
  - **Returns:**  
    - `true` if the operation was successful, otherwise `false`.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/FileUtils.hpp"

int main() {
    if (FileUtils::Rename("/home/user/file.txt", "/home/user/file_renamed.txt")) {
        std::cout << "File renamed successfully." << std::endl;
    } else {
        std::cerr << "Failed to rename file." << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { renameFile } from "@kingmaj0r/libfm/lib";

if (renameFile('/home/user/file.txt', '/home/user/file_renamed.txt')) {
    console.log('File renamed successfully.');
} else {
    console.log('Failed to rename file.');
}
```

:::
