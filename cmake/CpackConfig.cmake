set(CPACK_PACKAGE_NAME "Filerix")
set(CPACK_PACKAGE_VENDOR "Filesverse")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Filerix - File Management Library")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Filerix")
set(CPACK_PACKAGE_URL "https://github.com/filesverse/filerix")

set(CPACK_PACKAGE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

if(WIN32)
  if(NOT EXISTS "${ZLIB_LIBRARY}")
    message(FATAL_ERROR "zlib1.dll not found at ${ZLIB_LIBRARY}. Ensure Zlib is installed correctly via vcpkg.")
  endif()

  install(FILES ${ZLIB_LIBRARY} DESTINATION lib)

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CPACK_INSTALL_PREFIX "C:\\\\Program Files\\\\${CPACK_PACKAGE_NAME}")
  else()
    set(CPACK_INSTALL_PREFIX "C:\\\\Program Files (x86)\\\\${CPACK_PACKAGE_NAME}")
  endif()

  set(CPACK_MUI_ICON "${CMAKE_SOURCE_DIR}/windows/filerix.ico")
  set(CPACK_MUI_UNICON "${CMAKE_SOURCE_DIR}/windows/filerix.ico")

  set(CPACK_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  set(CPACK_SOURCE_DIR "${CMAKE_SOURCE_DIR}") 
  
  set(CPACK_MODULE_PATH "${CMAKE_SOURCE_DIR}/windows")

  set(CPACK_SCRIPT_PATH "${CMAKE_SOURCE_DIR}")
endif()

include(CPack)
