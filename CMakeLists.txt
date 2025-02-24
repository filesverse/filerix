cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(filerix VERSION 1.0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(ZLIB REQUIRED)

add_subdirectory(src)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/filerix.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/filerix.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/filerix.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
