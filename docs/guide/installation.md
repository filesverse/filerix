# **Installation**

::: warning Work in Progress  
This library is currently under development and may be subject to changes.  
:::  

## **Installation**  

### **Fedora (Recommended) – Install via Copr**  

For Fedora users, `filerix` is available in the Copr repository. You can install it directly with:  

```sh
sudo dnf copr enable kingmaj0r/filesverse
sudo dnf install filerix filerix-devel
```  

This will install both the library and development headers required for building projects with `filerix`.  

### **Manual Build (For Other Distributions)**  

For distributions other than Fedora, you will need to build and install `filerix` manually.  

#### **1. Install Dependencies**  

::: code-group  

```sh [<i class="devicon-fedora-plain colored"></i> Fedora]  
sudo dnf install meson gcc-c++ clang  
```  

```sh [<i class="devicon-ubuntu-plain colored"></i> Ubuntu]  
sudo apt install meson g++ clang  
```  

```sh [<i class="devicon-archlinux-plain colored"></i> Arch]  
sudo pacman -Syu meson g++ clang  
```  

:::  

#### **2. Clone the Repository**  

```sh
git clone https://github.com/KingMaj0r/filerix.git
cd filerix
```  

#### **3. Build the Project**  

```sh
meson setup builddir
meson compile -C builddir
```  

#### **4. Install the Library**  

```sh
meson install -C builddir
```  

The headers will be installed in `/usr/local/include/filerix`, and the compiled library will be placed in your system's default library directory (e.g., `/usr/local/lib`).  

## **Installation via Nix**  

To build and install the library using **Nix**, use the provided `flake.nix` file:  

1. Ensure **Nix** is installed. If not, follow the [official installation guide](https://nixos.org/download.html).  
2. Build the library:  

```sh
nix build .#filerix
```  

3. The resulting library will be found in the `result` symlink. To enter a development environment with `filerix` and its dependencies, run:  

```sh
nix develop
```  