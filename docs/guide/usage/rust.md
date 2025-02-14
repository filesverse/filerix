# <i class="devicon-rust-plain colored" style="color:#CE422B;"></i> **Rust Usage with Cargo**  

## **Prerequisites**  

Before proceeding, ensure the following are installed and set up:  

1. **Rust Toolchain**: Install Rust using [rustup](https://rustup.rs/).  
2. **filerix**: Ensure the `filerix` library and its development files are installed.  

## **Installing Dependencies**  

Depending on your distribution, install Rust and the required build tools:  

:::code-group  

```sh [<i class="devicon-fedora-plain colored"></i> Fedora]  
sudo dnf install cargo rustc
```  

```sh [<i class="devicon-ubuntu-plain colored"></i> Ubuntu]  
sudo apt install cargo rustc
```  

```sh [<i class="devicon-archlinux-plain colored"></i> Arch]  
sudo pacman -Syu cargo rust
```  

:::  

## **Creating a Rust Project**  

1. Create a new Rust project:  
   ```bash  
   cargo new example  
   cd example  
   ```  

2. Add `filerix` to your `Cargo.toml`:  
   ```toml  
   [dependencies]  
   filerix = "0.1"
   ``` 
## **Building the Example**  

1. Compile the project with Cargo:  
   ```bash  
   cargo build --release  
   ```  
   This will generate the executable in `target/release/example`.  

## **Running the Example**  

1. Run the compiled binary:  
   ```bash  
   ./target/release/example  
   ```  
2. The example will fetch and log the disk usage information for the specified path (`/` by default). You should see output like this:  
   ```bash  
   Used space: <used_bytes> bytes  
   Total space: <total_bytes> bytes  
   ```  

## **Understanding the Code**  

The Rust code interacts with the `filerix` library to fetch the disk usage of a given directory (`/` by default). Here’s a quick breakdown of the key components:  

- **`get_drive_usage()`**: Retrieves the disk usage details (used space and total space) for the given path.  
- **`println!`**: Prints the result to the console.  

This example demonstrates how to use `filerix` for filesystem operations and integrate it into a Rust project with Cargo.  

## **Common issues**

- If you encounter issues with library loading (e.g., `filerix.so` not found), ensure the library path is correctly set:
  ```bash
  export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
  ```