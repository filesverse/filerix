set(CPACK_PACKAGE_NAME "Filerix")
set(CPACK_PACKAGE_VENDOR "Filesverse")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Filerix - File Management Library")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Filerix")
set(CPACK_PACKAGE_URL "https://github.com/filesverse/filerix")

set(CPACK_PACKAGE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

find_package(ZLIB REQUIRED)

if(WIN32)
  set(ZLIB_DLL "/usr/x86_64-w64-mingw32/sys-root/mingw/bin/zlib1.dll")

  if(NOT EXISTS "${ZLIB_DLL}")
    message(FATAL_ERROR "zlib1.dll not found at ${ZLIB_DLL}. Ensure Zlib is installed correctly via vcpkg.")
  endif()

  install(FILES "${ZLIB_DLL}" DESTINATION lib)

  set(CPACK_MUI_ICON "${CMAKE_SOURCE_DIR}/windows/filerix.ico")
  set(CPACK_MUI_UNICON "${CMAKE_SOURCE_DIR}/windows/filerix.ico")

  set(CPACK_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  set(CPACK_SOURCE_DIR "${CMAKE_SOURCE_DIR}") 
  
  set(CPACK_MODULE_PATH "${CMAKE_SOURCE_DIR}/windows")

  set(CPACK_SCRIPT_PATH "${CMAKE_SOURCE_DIR}")
endif()

include(CPack)
