REPO_URL = https://github.com/filesverse/filerix.git
VCPKG_REPO = https://github.com/microsoft/vcpkg.git
PREFIX ?= /usr/local

all: install

install: build
	sudo cmake --install build --prefix=$(PREFIX) || { echo "Installation failed"; exit 1; }
	echo "Installation and build complete!"

build: check-vcpkg
	@echo "Bootstrapping vcpkg..."
	./vcpkg/bootstrap-vcpkg.sh || { echo "Failed to bootstrap vcpkg"; exit 1; }
	@echo "Installing dependencies with vcpkg..."
	./vcpkg/vcpkg --feature-flags=manifests install --triplet x64-linux-release || { echo "Failed to install dependencies"; exit 1; }
	@echo "Generating build files with CMake..."
	mkdir -p build && cd build && cmake .. || { echo "Failed to generate CMake build files"; exit 1; }
	@echo "Building the project..."
	cmake --build build --parallel || { echo "Build failed"; exit 1; }
	@echo "Build complete!"

check-vcpkg:
	@if [ ! -d "./vcpkg" ] || [ -z "$$(ls -A ./vcpkg 2>/dev/null)" ]; then \
		echo "vcpkg is missing or empty. Cloning vcpkg..."; \
		git clone $(VCPKG_REPO) vcpkg || { echo "Failed to clone vcpkg"; exit 1; }; \
	fi

uninstall:
	@echo "Removing installed files..."
	sudo rm -f $(PREFIX)/lib64/libfilerix.so
	sudo rm -f $(PREFIX)/lib64/pkgconfig/filerix.pc
	sudo rm -rf $(PREFIX)/include/filerix
	sudo rm -rf $(PREFIX)/share/filerix
	@echo "Uninstallation complete!"

clean:
	@echo "Cleaning build directory..."
	rm -rf build
	rm -rf vcpkg_installed
	@echo "Clean complete!"

.PHONY: all install build uninstall clean
