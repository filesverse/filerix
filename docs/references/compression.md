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

```cpp [<i class="devicon-cplusplus-plain colored"></i> C++]
#include <iostream>
#include "filerix/FileSystem/FileUtils.h"

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

```c [<i class="devicon-c-plain colored"></i> C]
#include <stdio.h>
#include "filerix/FileSystem/FileUtils.h"

int main() {
  const char *source = "/path/to/file.txt";
  const char *destination = "/path/to/file.gz";

  if (Compress(source, destination)) {
    printf("File compressed successfully.\n");
  } else {
    fprintf(stderr, "Compression failed.\n");
  }

  return 0;
}
```

```javascript [<i class="devicon-nodejs-plain colored"></i> Node.js]
import { compressFile } from "@kingmaj0r/filerix/lib";

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

```cpp [<i class="devicon-cplusplus-plain colored"></i> C++]
#include <iostream>
#include "filerix/FileSystem/FileUtils.h"

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

```c [<i class="devicon-c-plain colored"></i> C]
#include <stdio.h>
#include "filerix/FileSystem/FileUtils.h"

int main() {
  const char *source = "/path/to/file.gz";
  const char *destination = "/path/to/file.txt";

  if (Decompress(source, destination)) {
    printf("File decompressed successfully.\n");
  } else {
    fprintf(stderr, "Decompression failed.\n");
  }

  return 0;
}
```

```javascript [<i class="devicon-nodejs-plain colored"></i> Node.js]
import { decompressFile } from "@kingmaj0r/filerix/lib";

if (decompressFile('/path/to/file.gz', '/path/to/file.txt')) {
    console.log('File decompressed successfully.');
} else {
    console.log('Decompression failed.');
}
```

:::