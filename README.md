
# FM (File Manager) - WIP (Work in Progress)

FM is a lightweight and modular file manager library that provies all the essentials written in C++.

## Installation

### Prerequisites

- **Meson Build System**: To build and install the project.
- **C++17 compatible compiler** (e.g., `g++`, `clang++`).

### Clone the repository

```bash
git clone https://github.com/KingMaj0r/libfm.git
cd libfm
```

### Build the project

```bash
meson setup builddir
meson compile -C builddir
```

### Install the library

To install the library and headers:

```bash
meson install -C builddir
```

The headers will be installed in the `/usr/local/include/libfm` directory, and the library will be installed to your system's default library directory.

## Usage

### Linking the Library

To use the FM library in your own project, add the following to your `meson.build`:

```meson
dependency('libfm', required: true)
```

Then link the library in your C++ project. Be sure to include the appropriate headers from `include/libfm`.

## Contributing

We welcome contributions! Please feel free to open issues, submit bug reports, and create pull requests. Here are some guidelines to follow when contributing:

1. Fork the repository and create your branch from `main`.
2. Make your changes.
3. Write tests for any new functionality.
4. Ensure that the code follows the existing style and passes all tests.
5. Submit a pull request with a clear description of your changes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Meson](https://mesonbuild.com) - Build system
- [C++](https://isocpp.org) - Programming language used
- [GitHub](https://github.com) - Repository hosting
