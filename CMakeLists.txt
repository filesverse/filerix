cmake_minimum_required(VERSION 3.16)

project(filerix VERSION 1.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

if(CMAKE_CROSSCOMPILING OR WIN32)
  set(TARGET_OS "Windows")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-mingw-dynamic/include")
    set(ZLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-mingw-dynamic/bin/libzlib1.dll")
  else()
    set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x86-mingw-dynamic/include")
    set(ZLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x86-mingw-dynamic/bin/libzlib1.dll")
  endif()
else()
  set(TARGET_OS "Linux")
endif()

find_package(PkgConfig REQUIRED)

find_package(ZLIB REQUIRED)

if(TARGET_OS STREQUAL "Linux")
  pkg_check_modules(UDEV REQUIRED libudev)
  pkg_check_modules(DBUS REQUIRED dbus-1)
endif()

if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
endif()

add_subdirectory(src)

if(TARGET_OS STREQUAL "Linux")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/filerix.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/filerix.pc @ONLY)
  
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/filerix.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/CpackConfig.cmake)

if(WIN32)
  add_custom_target(installer
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config Release
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 ${CMAKE_CPACK_COMMAND} -D CPACK_OUTPUT_FILE_PREFIX=${CMAKE_BINARY_DIR} -G NSIS
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()
