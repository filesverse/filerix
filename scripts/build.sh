#!/bin/bash

echo "Bootstrapping vcpkg..."
./vcpkg/bootstrap-vcpkg.sh || { echo "Failed to bootstrap vcpkg"; exit 1; }

echo "Installing dependencies with vcpkg..."
./vcpkg/vcpkg --feature-flags=manifests install --triplet x64-linux-release || { echo "Failed to install dependencies"; exit 1; }

echo "Generating build files with CMake..."
mkdir build && cd build
cmake .. || { echo "Failed to generate CMake build files"; exit 1; }

echo "Building the project..."
cmake --build . --parallel || { echo "Build failed"; exit 1; }
cd ..

echo "Installation and build complete!"
