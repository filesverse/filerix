#!/bin/bash

echo "Cloning the repository with submodules..."
git clone --recurse-submodules https://github.com/filesverse/filerix.git
cd filerix || { echo "Failed to enter the project directory"; exit 1; }

echo "Bootstrapping vcpkg..."
./vcpkg/bootstrap-vcpkg.sh || { echo "Failed to bootstrap vcpkg"; exit 1; }

echo "Installing dependencies with vcpkg..."
./vcpkg/vcpkg --feature-flags=manifests install || { echo "Failed to install dependencies"; exit 1; }

echo "Generating build files with cmake..."
cmake -B build -S . || { echo "Failed to generate cmake build files"; exit 1; }

echo "Building the project..."
cmake --build build || { echo "Build failed"; exit 1; }

echo "Installing the built project..."
sudo make install || { echo "Installation failed"; exit 1; }

echo "Installation and build complete!"