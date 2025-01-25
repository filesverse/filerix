# Installation

::: warning Work in Progress 
This library is currently under development and may be subject to changes. 
:::

## Installation

### Prerequisites

- **Meson Build System**: To build and install the project.
- **C++17 compatible compiler** (e.g., `g++`, `clang++`).

1. install dependencies
:::code-group

```sh [<i class="devicon-fedora-plain"></i> Fedora]
sudo dnf install meson gcc-c++ clang
```

```sh [<i class="devicon-ubuntu-plain"></i> Ubuntu]
sudo apt install meson g++ clang
```

```sh [<i class="devicon-archlinux-plain"></i> Arch]
sudo pacman -Syu meson g++ clang
```

:::

2. Clone the repository

```bash
git clone https://github.com/KingMaj0r/libfm.git
cd libfm
```

3. Build the project

```bash
meson setup builddir
meson compile -C builddir
```

4. Install the library

```bash
meson install -C builddir
```

The headers will be installed in the `/usr/local/include/libfm` directory, and the library will be installed to your system's default library directory.

## Installation via Nix

To build and install the library using Nix, you can use the provided `flake.nix` file:

1. Ensure you have Nix installed on your system. If not, follow the [official installation guide](https://nixos.org/download.html).
2. Run the following command to build the library:

```bash
nix build .#libfm
```

3. The resulting library can be found in the `result` symlink. To use it in your environment, you can run:

```bash
nix develop
```

This will create a development shell with the library and its dependencies set up.
