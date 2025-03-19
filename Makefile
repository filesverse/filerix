VCPKG_REPO = https://github.com/microsoft/vcpkg.git
OS := $(shell uname -s)
PREFIX ?= /usr/local

ifeq ($(OS), MINGW64_NT-10.0)
  PREFIX := C:/Program Files/Filerix
endif

all: build-all

# ---- Installation Targets ----
install: build-linux
	@echo "Installing Filerix..."
	cmake --install build/linux --prefix="$(PREFIX)" || { echo "Installation failed"; exit 1; }
	@echo "Installation complete!"

install-win: build-win
	@echo "Installing Filerix (Windows)..."
	cmake --install build/windows-x64 --prefix="$(PREFIX)" || { echo "Windows installation failed"; exit 1; }
	@echo "Windows installation complete!"

install-win32: build-win32
	@echo "Installing Filerix (Windows 32-bit)..."
	cmake --install build/windows-x86 --prefix="$(PREFIX)" || { echo "Windows 32-bit installation failed"; exit 1; }
	@echo "Windows 32-bit installation complete!"

# ---- Build Targets ----
build-all: build-linux build-win build-win32
	@echo "All builds completed successfully!"

build-linux: check-vcpkg
	@echo "Building for Linux..."
	./vcpkg/vcpkg install --triplet x64-linux-release
	cmake -B build/linux
	cmake --build build/linux --parallel

build-win: check-vcpkg
	@echo "Building for Windows (x64)..."
	./vcpkg/vcpkg install --triplet x64-mingw-dynamic
	cmake -B build/windows-x64 -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-mingw64.cmake
	cmake --build build/windows-x64 --parallel --config Release

build-win32: check-vcpkg
	@echo "Building for Windows (x86)..."
	./vcpkg/vcpkg install --triplet x86-mingw-dynamic
	cmake -B build/windows-x86 -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-mingw32.cmake
	cmake --build build/windows-x86 --parallel --config Release

# ---- Packaging ----
build-bin: generate-changelog build-all
	@echo "Generating RPM and DEB packages..."
	( cd build/linux && cpack -G "DEB;RPM" ) || { echo "Package generation failed"; exit 1; }
	
	@echo "Generating Windows NSIS installer..."
	( cd build/windows-x64 && cpack -G NSIS ) || { echo "NSIS package generation failed"; exit 1; }
	( cd build/windows-x86 && cpack -G NSIS ) || { echo "NSIS package generation failed"; exit 1; }

	@rm -f changelog-deb changelog-rpm
	@echo "Package generation complete!"

# ---- Changelog Generation ----
generate-changelog:
	@echo "Generating changelog..."
	@echo "filerix (v1.1.0-$(shell git rev-list --count HEAD)) unstable; urgency=medium" > changelog-deb
	@git log --pretty=format:"  * %s" --reverse >> changelog-deb
	@echo "\n -- $(shell git config user.name) <$(shell git config user.email)>  $(shell date -R)" >> changelog-deb
	@echo "%changelog" > changelog-rpm
	@git log --pretty=format:"* %ad $(shell git config user.name) - %s" --date=short --reverse >> changelog-rpm
	@echo "Changelog generated successfully!"

# ---- Utility Targets ----
check-vcpkg:
	@if [ ! -d "./vcpkg" ] || [ -z "$$(ls -A ./vcpkg 2>/dev/null)" ]; then \
		echo "vcpkg is missing. Cloning..."; \
		git clone $(VCPKG_REPO) vcpkg || { echo "Failed to clone vcpkg"; exit 1; }; \
		./vcpkg/bootstrap-vcpkg.sh; \
	fi

clean:
	@echo "Cleaning build directories..."
	rm -rf build vcpkg_installed
	@echo "Clean complete!"

.PHONY: all install install-win build build-win build-win32 build-bin clean generate-changelog