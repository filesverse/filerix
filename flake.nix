{
  description = "File manager library that provides all the essentials.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachSystem [ "x86_64-linux" ] (system: let
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      packages.filerix = pkgs.stdenv.mkDerivation rec {
        pname = "filerix";
        version = "1.0.2";

        src = ./.;

        nativeBuildInputs = [
          pkgs.cmake
          pkgs.pkg-config
          pkgs.zlib
        ];

        buildInputs = [
          pkgs.gcc
          pkgs.libcxx
        ];

        installPhase = ''
          echo "Bootstrapping vcpkg..."
          ./vcpkg/bootstrap-vcpkg.sh

          echo "Installing dependencies with vcpkg..."
          ./vcpkg/vcpkg --feature-flags=manifests install

          echo "Generating build files with cmake..."
          cmake -B build -S .

          echo "Building the project..."
          cmake --build build

          echo "Installing the built project..."
          sudo make install
        '';

        meta = with pkgs.lib; {
          description = "File manager library that provides all the essentials.";
          license = licenses.mit;
          platforms = platforms.linux;
        };
      };
    });
}
