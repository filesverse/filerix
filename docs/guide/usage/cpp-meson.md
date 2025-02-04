# **C++ Usage with Meson**

## **Prerequisites**

Before proceeding, ensure the following are installed and set up:

1. **C++ Compiler**: Ensure you have a C++ compiler installed. For example, `g++` on Linux.

2. **Meson**: Install Meson. If you don't have it already:
   
3. **filerix**: Ensure the `filerix` library and its development files are installed.

## **Installing Dependencies**

Depending on your distribution, run one of the following commands to install the required build tools:

:::code-group

```sh [<i class="devicon-fedora-plain"></i> Fedora]
sudo dnf install meson ninja
```

```sh [<i class="devicon-ubuntu-plain"></i> Ubuntu]
sudo apt install meson ninja
```

```sh [<i class="devicon-archlinux-plain"></i> Arch]
sudo pacman -Syu meson ninja
```

:::

## **Building the Example with Meson**

1. Navigate to the `example/cpp+meson` directory:
   ```bash
   cd example/cpp+meson
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

The C++ code interacts with the `filerix` library to fetch the disk usage of a given directory (`/` by default). Here’s a quick breakdown of the key components:

- **DriveUtils::GetDriveUsage**: This function retrieves the disk usage details (used space and total space) for the given path.
- **Logger::Info**: This utility logs information (like the disk usage) to a log file or the console.
- **std::cout**: The result is printed to the console for the user to see.

This simple program is designed to showcase how to use `filerix` for filesystem operations and how to integrate it into a C++ project with CMake and Meson.

## **Common Issues**

- **Missing `filerix.so` (Shared Library)**:
  If you encounter errors about missing libraries (e.g., `filerix.so`), ensure the library path is correctly set:
  ```bash
  export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
  ```

  If `filerix` was installed manually, ensure the path where it's located is included in `LD_LIBRARY_PATH`.

- **Meson or Ninja Errors**:
  If you run into issues during the Meson configuration step, verify that you have the correct `filerix` development package installed and that it's being detected by Meson.

  You can also try running:
  ```bash
  meson .. --reconfigure
  ```
  This will force Meson to reconfigure the project and show more detailed output to help troubleshoot.