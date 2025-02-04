# **Drives**

## getDrives()
- **`getDrives(): DriveInfo[]`**  
  - **Description:** Returns a list of all available drives on the system.  
  - **Returns:**  
    - An array of `DriveInfo` objects, each containing:
      - `device` (string): The device path.
      - `status` (string): `mounted` or `unmounted`.
      - `unmountable` (boolean): Whether the drive can be unmounted.
      - `mountPoint` (string): The mount point.
      - `partition` (string): The partition path.
      - `fsType` (string): The partition type.
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  auto drives = DriveUtils::GetDrives();

  if (drives.empty()) {
    std::cout << "No drives found or an error occurred." << std::endl;
    return 1;
  }
  
  for (const auto& drive : drives) {
    std::cout << "Device: " << drive.device 
              << ", Status: " << drive.status 
              << ", Mount Point: " << drive.mountPoint 
              << std::endl;
  }

  return 0;
}
```

```c [C]
#include <stdio.h>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  DriveInfo *drives = GetDrives();

  if (drives == NULL) {
    printf("No drives found or an error occurred.\n");
    return 1;
  }

  for (size_t i = 0; i < numDrives; ++i) {
    printf("Device: %s, Status: %s, Mount Point: %s\n",
           drives[i].device,
           drives[i].status,
           drives[i].mountPoint);
  }

  return 0;
}
```

```javascript [Node.js]
import { getDrives } from "@kingmaj0r/filerix/lib";

const drives = getDrives();

if (!drives) {
  console.error("No drives found or an error occurred.");
}

drives.forEach(drive => {
    console.log(`Device: ${drive.device}, Status: ${drive.status}, Mount Point: ${drive.mountPoint}`);
});
```

:::

## getDriveUsage()
- **`getDriveUsage(drivePath: string): { used_space: number; total_space: number; }`**  
  - **Description:** Retrieves the total and used space of a specified drive.  
  - **Parameters:**  
    - `drivePath` (string): The path to the drive (e.g., `/` or `/dev/sda1`).  
  - **Returns:**  
    - An object with:
      - `used_space`: The used space in bytes.  
      - `total_space`: The total space in bytes.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  const char *device = "/dev/sdX";

  auto [usedBytes, totalBytes] = DriveUtils::GetDriveUsage(device);

  if (usedBytes || totalBytes) {
    std::cout << "Used space: " << usedBytes << " bytes" << std::endl;
    std::cout << "Total space: " << totalBytes << " bytes" << std::endl;
  } else {
    std::cerr << "Failed to get drive usage.";
  }

  return 0;
}
```

```c [C]
#include <stdio.h>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  const char *device = "/dev/sdX";

  DriveUsage usage = GetDriveUsage(device);

  if (usage.used || usage.total) {
    printf("Used space: %llu bytes\n", usage.used);
    printf("Total space: %llu bytes\n", usage.total);
  } else {
    fprintf(stderr, "Failed to get drive usage.\n");
  }

  return 0;
}
```

```javascript [Node.js]
import { getDriveUsage } from "@kingmaj0r/filerix/lib";

const usage = filerix.getDriveUsage('/dev/sdX');
console.log(`Used space: ${usage.used_space} bytes`);
console.log(`Total space: ${usage.total_space} bytes`);
```

:::

## mountDrive()
- **`mountDrive(drivePath: string): boolean`**  
  - **Description:** Mounts the specified drive to the given mount point.  
  - **Parameters:**  
    - `drivePath` (string): The path to the drive (e.g., `/dev/sda1`).  
  - **Returns:**  
    - A boolean indicating whether the mounting was successful.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  const char *device = "/dev/sdX";

  bool success = DriveUtils::MountDrive(device);

  if (success) {
    std::cout << "Drive mounted successfully." << std::endl;
  } else {
    std::cout << "Drive mounting failed for " << device << std::endl;
  }

  return 0;
}
```

```c [C]
#include <stdio.h>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  const char *device = "/dev/sdX";

  int success = MountDrive(device);

  if (success) {
    printf("Drive unmounted successfully.\n");
  } else {
    printf("Drive unmounting failed for %s.\n", device);
  }

  return 0;
}
```

```javascript [Node.js]
import { mountDrive } from "@kingmaj0r/filerix/lib";

const device = "/dev/sdX";

const success = mountDrive(device);
if (success) {
  console.log("Drive mounted successfully.");
} else {
  console.log("Drive mounting failed for ", device);
}
```

:::

## unmountDrive()
- **`unmountDrive(drivePath: string): boolean`**  
  - **Description:** Unmounts the specified drive.  
  - **Parameters:**  
    - `drivePath` (string): The path to the drive to be unmounted (e.g., `/dev/sda1`).  
  - **Returns:**  
    - A boolean indicating whether the unmounting was successful.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  const char *device = "/dev/sdX";

  bool success = DriveUtils::UnmountDrive(device);
  
  if (success) {
    std::cout << "Drive unmounted successfully." << std::endl;
  } else {
    std::cout << "Drive unmounting failed for " << device << std::endl;
  }

  return 0;
}
```

```c [C]
#include <stdio.h>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  const char *device = "/dev/sdX";

  int success = UnmountDrive(device);

  if (success) {
    printf("Drive unmounted successfully.\n");
  } else {
    printf("Drive unmounting failed for %s\n", device);
  }

  return 0;
}

```

```javascript [Node.js]
import { unmountDrive } from "@kingmaj0r/filerix/lib";

const device = "/dev/sdX";

const success = unmountDrive(device);
if (success) {
  console.log("Drive unmounted successfully.");
} else {
  console.log("Drive unmounting failed for ", device);
}
```

:::

## getDeviceLabelOrUUID()
- **`getDeviceLabelOrUUID(drivePath: string): string`**  
  - **Description:** Gets the device label or UUID.
  - **Parameters:**  
    - `drivePath` (string): The path to the drive (e.g., `/dev/sda1`).  
  - **Returns:**  
    - A string containing the drive's label or UUID.
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  const char *device = "/dev/sdX";

  std::string label_or_uuid = DriveUtils::GetDeviceLabelOrUUID(device);

  if (!label_or_uuid.empty()) {
    std::cout << "Label or UUID for device " << device << ": " << label_or_uuid << std::endl;
  } else {
    std::cout << "No label or UUID found for device " << device << "." << std::endl;
  }

  return 0;
}
```

```c [C]
#include <stdio.h>
#include "filerix/FileSystem/DriveUtils.h"

int main() {
  const char *device = "/dev/sdX";

  const char *label_or_uuid = GetDeviceLabelOrUUID(device);

  if (label_or_uuid) {
    printf("Label or UUID for device %s: %s\n", device, label_or_uuid);
  } else {
    printf("No label or UUID found for device %s\n", device);
  }

  return 0;
}
```

```javascript [Node.js]
import { getDeviceLabelOrUUID } from "@kingmaj0r/filerix/lib";

const device = "/dev/sdX"

const name = getDeviceLabelOrUUID(device);

if (name) {
  console.log("Label or UUID for device {device}: {name}");
} else {
  console.log("No label or UUID found for device ", device);
}
```

:::