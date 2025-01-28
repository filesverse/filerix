# **Drives**

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
#include "libfm/FileSystem/DriveUtils.hpp"

int main() {
    try {
        auto [usedBytes, totalBytes] = DriveUtils::GetDriveUsage("/");
        std::cout << "Used space: " << usedBytes << " bytes" << std::endl;
        std::cout << "Total space: " << totalBytes << " bytes" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { getDriveUsage } from "@kingmaj0r/libfm/lib";

const usage = libfm.getDriveUsage('/dev/sdX');
console.log(`Used space: ${usage.used_space} bytes`);
console.log(`Total space: ${usage.total_space} bytes`);
```

:::

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
#include "libfm/FileSystem/DriveUtils.hpp"

int main() {
    try {
        auto drives = DriveUtils::GetDrives();
        for (const auto& drive : drives) {
            std::cout << "Device: " << drive.device 
                      << ", Status: " << drive.status 
                      << ", Mount Point: " << drive.mountPoint 
                      << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { getDrives } from "@kingmaj0r/libfm/lib";

const drives = getDrives();
drives.forEach(drive => {
    console.log(`Device: ${drive.device}, Status: ${drive.status}, Mount Point: ${drive.mountPoint}`);
});
```

:::

## getDriveInfo()
- **`getDriveInfo(drivePath: string): DriveInfo`**  
  - **Description:** Returns detailed information about a specific drive.  
  - **Parameters:**  
    - `drivePath` (string): The path to the drive (e.g., `/` or `/dev/sda1`).  
  - **Returns:**  
    - A `DriveInfo` object containing:
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
#include "libfm/FileSystem/DriveUtils.hpp"

int main() {
    try {
        auto driveInfo = DriveUtils::GetDriveInfo("/dev/sdX");
        std::cout << "Device: " << driveInfo.device 
                  << ", Status: " << driveInfo.status 
                  << ", Mount Point: " << driveInfo.mountPoint 
                  << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { getDriveInfo } from "@kingmaj0r/libfm/lib";

const driveInfo = getDriveInfo('/dev/sdX');
console.log(`Device: ${driveInfo.device}, Status: ${driveInfo.status}, Mount Point: ${driveInfo.mountPoint}`);
```

:::

## mountDrive()
- **`mountDrive(drivePath: string, mountPoint: string): boolean`**  
  - **Description:** Mounts the specified drive to the given mount point.  
  - **Parameters:**  
    - `drivePath` (string): The path to the drive (e.g., `/dev/sda1`).  
    - `mountPoint` (string): The path where the drive will be mounted (e.g., `/mnt/mydrive`).  
  - **Returns:**  
    - A boolean indicating whether the mounting was successful.  
  - **Examples:**

:::code-group

```cpp [C++]
#include <iostream>
#include "libfm/FileSystem/DriveUtils.hpp"

int main() {
    try {
        bool success = DriveUtils::MountDrive("/dev/sdX", "/mnt/mydrive");
        if (success) {
            std::cout << "Drive mounted successfully." << std::endl;
        } else {
            std::cout << "Drive mounting failed." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { mountDrive } from "@kingmaj0r/libfm/lib";

const success = mountDrive('/dev/sdX', '/mnt/mydrive');
if (success) {
    console.log("Drive mounted successfully.");
} else {
    console.log("Drive mounting failed.");
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
#include "libfm/FileSystem/DriveUtils.hpp"

int main() {
    try {
        bool success = DriveUtils::UnmountDrive("/dev/sdX");
        if (success) {
            std::cout << "Drive unmounted successfully." << std::endl;
        } else {
            std::cout << "Drive unmounting failed." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

```javascript [Node.js]
import { unmountDrive } from "@kingmaj0r/libfm/lib";

const success = unmountDrive('/dev/sdX');
if (success) {
    console.log("Drive unmounted successfully.");
} else {
    console.log("Drive unmounting failed.");
}
```

:::
