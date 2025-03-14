VCPKG_REPO = https://github.com/microsoft/vcpkg.git
OS := $(shell uname -s)

ifeq ($(OS), MINGW64_NT-10.0)
    PREFIX ?= C:/Program Files/Filerix
else
    PREFIX ?= /usr/local
endif

all: install

install: build
	@echo "Installing Filerix..."
	cmake --install build --prefix="$(PREFIX)" || { echo "Installation failed"; exit 1; }
	@echo "Installation complete!"

install-win: build-win
	@echo "Installing Filerix (Windows)..."
	cmake --install build-win --prefix="$(PREFIX)" || { echo "Windows installation failed"; exit 1; }
	@echo "Windows installation complete!"

install-win: build-win32
	@echo "Installing Filerix (Windows)..."
	cmake --install build-win32 --prefix="$(PREFIX)" || { echo "Windows installation failed"; exit 1; }
	@echo "Windows installation complete!"

build: check-vcpkg
	@echo "Bootstrapping vcpkg..."
	./vcpkg/bootstrap-vcpkg.sh || { echo "Failed to bootstrap vcpkg"; exit 1; }
	@echo "Installing Linux dependencies..."
	./vcpkg/vcpkg --feature-flags=manifests install --triplet x64-linux-release || { echo "Failed to install Linux dependencies"; exit 1; }
	@echo "Generating Linux build files with CMake..."
	cmake -B build || { echo "Failed to generate Linux CMake build files"; exit 1; }
	@echo "Building for Linux..."
	cmake --build build --parallel || { echo "Linux build failed"; exit 1; }

build-win: check-vcpkg
	@echo "Setting up MinGW cross-compilation..."
	./vcpkg/vcpkg --feature-flags=manifests install --triplet x64-mingw-static || { echo "Failed to install Windows dependencies"; exit 1; }
	@echo "Generating Windows build files with CMake..."
	cmake -B build-win -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-mingw64.cmake || { echo "Failed to generate Windows CMake build files"; exit 1; }
	@echo "Building for Windows..."
	cmake --build build-win --parallel --config Release --target installer || { echo "Windows build failed"; exit 1; }

build-win32: check-vcpkg
	@echo "Setting up MinGW cross-compilation..."
	./vcpkg/vcpkg --feature-flags=manifests install --triplet x86-mingw-static || { echo "Failed to install Windows dependencies"; exit 1; }
	@echo "Generating Windows build files with CMake..."
	cmake -B build-win -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-mingw32.cmake || { echo "Failed to generate Windows CMake build files"; exit 1; }
	@echo "Building for Windows..."
	cmake --build build-win --parallel --config Release --target installer || { echo "Windows build failed"; exit 1; }

check-vcpkg:
	@if [ ! -d "./vcpkg" ] || [ -z "$$(ls -A ./vcpkg 2>/dev/null)" ]; then \
		echo "vcpkg is missing or empty. Cloning vcpkg..."; \
		git clone $(VCPKG_REPO) vcpkg || { echo "Failed to clone vcpkg"; exit 1; }; \
	fi

uninstall:
	@echo "Removing installed files..."
	rm -f "$(PREFIX)/lib64/libfilerix.so"
	rm -f "$(PREFIX)/lib64/pkgconfig/filerix.pc"
	rm -rf "$(PREFIX)/include/filerix"
	rm -rf "$(PREFIX)/share/filerix"
	@echo "Uninstallation complete!"

uninstall-win:
	@echo "Removing Windows installed files..."
	rm -rf "$(PREFIX)"
	@echo "Windows uninstallation complete!"

clean:
	@echo "Cleaning build directories..."
	rm -rf build build-win vcpkg_installed
	@echo "Clean complete!"

.PHONY: all install install-win build build-win uninstall uninstall/win clean
