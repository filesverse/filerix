# <i class="devicon-c-plain colored"></i> **C Usage with CMake and Meson**

### **Prerequisites**

Before proceeding, ensure the following are installed and set up:

1. **C Compiler**: Ensure you have a C compiler installed. For example, `gcc` on Linux.
2. **filerix**: Ensure the `filerix` library and its development files are installed.

#### **CMake Prerequisites**

1. **CMake**: Install CMake. If you don't have it already:
   
#### **Meson Prerequisites**

2. **Meson**: Install Meson. If you don't have it already:

## **Installing Dependencies**

Depending on your distribution, run one of the following commands to install the required build tools:

:::code-group

```sh [<i class="devicon-fedora-plain colored"></i> Fedora]
sudo dnf install cmake make meson ninja
```

```sh [<i class="devicon-ubuntu-plain colored"></i> Ubuntu]
sudo apt install cmake make meson ninja
```

```sh [<i class="devicon-archlinux-plain colored"></i> Arch]
sudo pacman -Syu cmake make meson ninja
```

:::

## **Building the Example with CMake**

1. Navigate to the `example/c+cmake` directory:
   ```bash
   cd example/c+cmake
   ```

2. Create a build directory to keep the build files organized:
   ```bash
   mkdir build
   cd build
   ```

3. Run `cmake` to configure the project:
   ```bash
   cmake ..
   ```

4. Build the project with `make`:
   ```bash
   make
   ```

   After building, the executable `example` will be available in the `build` directory.

## **Building the Example with Meson**

1. Navigate to the `example/c+meson` directory:
   ```bash
   cd example/c+meson
   ```

2. Create a build directory to keep the build files organized:
   ```bash
   mkdir build
   cd build
   ```

3. Run `meson` to configure the project:
   ```bash
   meson ..
   ```

4. Build the project with `ninja`:
   ```bash
   ninja
   ```

   After building, the executable `example` will be available in the `build` directory.

## **Running the Example**

1. Once the build process completes, run the compiled executable:
   ```bash
   ./example
   ```

2. The example will fetch and log the disk usage information for the specified path (`/` by default). You should see output like this:
   ```bash
   Used space: <used_bytes> bytes
   Total space: <total_bytes> bytes
   ```

## **Understanding the Code**

The C code interacts with the `filerix` library to fetch the disk usage of a given directory (/ by default). Here’s a quick breakdown of the key components:

- **GetDriveUsage**: This function retrieves the disk usage details (used space and total space) for the given path.
- **printf**: The result is printed to the console for the user to see.

This simple program is designed to showcase how to use `filerix` for filesystem operations and how to integrate it into a C++ project with CMake and Meson.

## **Common Issues**

- **Missing `filerix.so` (Shared Library)**:
  If you encounter errors about missing libraries (e.g., `filerix.so`), ensure the library path is correctly set:
  ```bash
  export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
  ```

  If `filerix` was installed manually, ensure the path where it's located is included in `LD_LIBRARY_PATH`.
