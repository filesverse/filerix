cmake_minimum_required(VERSION 3.16)

project(filerix VERSION 1.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

include(${CMAKE_SOURCE_DIR}/cmake/Zlib.cmake)

if(CMAKE_CROSSCOMPILING OR WIN32)
  set(TARGET_OS "Windows")
else()
  set(TARGET_OS "Linux")
endif()

find_package(ZLIB REQUIRED)

if(TARGET_OS STREQUAL "Linux")
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(UDEV REQUIRED libudev)
  find_package(DBus1 CONFIG REQUIRED)
endif()

add_subdirectory(src)

if(TARGET_OS STREQUAL "Linux")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/filerix.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/filerix.pc @ONLY)
  
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/filerix.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

if(NOT DEFINED VCPKG_TARGET_TRIPLET)
  include(${CMAKE_SOURCE_DIR}/cmake/CpackConfig.cmake)
endif()