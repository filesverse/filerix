# Getting Started

Below are two examples to help you get started with **Node.js** or **C++**.

:::code-group

```cpp [<i class="devicon-cplusplus-plain colored"></i> C++]
#include <iostream>
#include <string>
#include "filerix/FileSystem/DriveUtils.hpp"

int main()
{
  std::string drivePath = "/";

  auto [usedBytes, totalBytes] = DriveUtils::GetDriveUsage(drivePath);

  std::cout << "Used space: " << usedBytes << " bytes" << std::endl;
  std::cout << "Total space: " << totalBytes << " bytes" << std::endl;

  return 0;
}
```

```c [<i class="devicon-c-plain colored"></i> C]
#include <stdio.h>
#include <string.h>
#include "filerix/FileSystem/DriveUtils.h"

int main()
{
  const char *drivePath = "/";

  DriveUsage usage = GetDriveUsage(drivePath);

  printf("Used space: %llu bytes\n", usage.used);
  printf("Total space: %llu bytes\n", usage.total);

  return 0;
}
```

```javascript [<i class="devicon-nodejs-plain colored"></i> Node.js]
import { getDriveUsage } from "@kingmaj0r/filerix/lib";

async function getDriveUsageFunc(drivePath) {
  try {
    const usage = await getDriveUsage(drivePath);

    const usedBytes = usage.used_space;
    const totalBytes = usage.total_space;

    console.log(`Used space: ${usedBytes} bytes`);
    console.log(`Total space: ${totalBytes} bytes`);
  } catch (error) {
    console.error('Error fetching drive usage:', error);
  }
}

const drivePath = '/dev/sdX';
getDriveUsageFunc(drivePath);
```

```rust [<i class="devicon-rust-plain colored" style="color:#CE422B;"></i> Rust]
use filerix::FileSystem::DriveUtils;

fn main() {
  let drive_path = "/dev/sda1";

  match DriveUtils::get_drive_usage(drive_path) {
    Some(usage) => {
      println!("Used space: {} bytes", usage.used);
      println!("Total space: {} bytes", usage.total);
    }
    None => {
      println!("Failed to retrieve drive usage for {}", drive_path);
    }
  }
}
```

:::