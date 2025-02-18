#!/bin/bash

TMP_DIR=$(mktemp -d) || { echo "Failed to create temp directory"; exit 1; }
echo "Using temporary directory: $TMP_DIR"

cd "$TMP_DIR" || { echo "Failed to enter temp directory"; exit 1; }

echo "Cloning the repository with submodules..."
git clone --recurse-submodules https://github.com/filesverse/filerix.git || { echo "Failed to clone repository"; exit 1; }

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
cd build && sudo make install || { echo "Installation failed"; exit 1; }

cd "$HOME" || { echo "Failed to return to home directory"; exit 1; }

echo "Removing temporary folder: $TMP_DIR"
rm -rf "$TMP_DIR" || { echo "Removing temp folder failed"; exit 1; }

echo "Installation and build complete!"
