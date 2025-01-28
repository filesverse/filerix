# Getting Started

Below are two examples to help you get started with **Node.js** or **C++**.

:::code-group

```cpp [C++]
#include <iostream>
#include <string>
#include "libfm/FileSystem/DriveUtils.hpp"
#include "libfm/utils/Logger.hpp"

int main()
{
  std::string drivePath = "/";

  auto [usedBytes, totalBytes] = DriveUtils::GetDriveUsage(drivePath);

  Logger::Info("Used space: " + std::to_string(usedBytes) + " bytes");
  Logger::Info("Total space: " + std::to_string(totalBytes) + " bytes");

  std::cout << "Used space: " << usedBytes << " bytes" << std::endl;
  std::cout << "Total space: " << totalBytes << " bytes" << std::endl;

  return 0;
}
```

```javascript [Node.js]
import { getDriveUsage } from "@kingmaj0r/libfm/lib";

async function getDriveUsageFunc(drivePath) {
  try {
    const usage = await libfm.getDriveUsage(drivePath);

    const usedBytes = usage.used_space;
    const totalBytes = usage.total_space;

    console.log(`Used space: ${usedBytes} bytes`);
    console.log(`Total space: ${totalBytes} bytes`);
  } catch (error) {
    console.error('Error fetching drive usage:', error);
  }
}

const drivePath = '/dev/sdX'; // or /dev/sdX
getDriveUsageFunc(drivePath);
```

:::