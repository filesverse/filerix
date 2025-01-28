# **Compression**

## compressFile()
- **`compressFile(source: string, destination: string): boolean`**  
  - **Description:** Compresses a file using gzip.  
  - **Parameters:**  
    - `source` (string): The file to compress.  
    - `destination` (string): The compressed file destination.  
  - **Returns:**  
    - `true` if successful, otherwise `false`.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/FileUtils.hpp"

int main() {
    const std::string source = "/path/to/file.txt";
    const std::string destination = "/path/to/file.gz";

    if (FileUtils::Compress(source, destination)) {
        std::cout << "File compressed successfully." << std::endl;
    } else {
        std::cerr << "Compression failed." << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { compressFile } from "@kingmaj0r/libfm/lib";

if (compressFile('/path/to/file.txt', '/path/to/file.gz')) {
    console.log('File compressed successfully.');
} else {
    console.log('Compression failed.');
}
```

:::

## decompressFile()
- **`decompressFile(source: string, destination: string): boolean`**  
  - **Description:** Decompresses a gzip file.  
  - **Parameters:**  
    - `source` (string): The compressed file path.  
    - `destination` (string): The output decompressed file path.  
  - **Returns:**  
    - `true` if successful, otherwise `false`.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/FileUtils.hpp"

int main() {
    const std::string source = "/path/to/file.gz";
    const std::string destination = "/path/to/file.txt";

    if (FileUtils::Decompress(source, destination)) {
        std::cout << "File decompressed successfully." << std::endl;
    } else {
        std::cerr << "Decompression failed." << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { decompressFile } from "@kingmaj0r/libfm/lib";

if (decompressFile('/path/to/file.gz', '/path/to/file.txt')) {
    console.log('File decompressed successfully.');
} else {
    console.log('Decompression failed.');
}
```

:::