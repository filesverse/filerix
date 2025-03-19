set(CPACK_PACKAGE_NAME "Filerix")
set(CPACK_PACKAGE_VENDOR "Filesverse")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Filerix - File Management Library")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Filerix")
set(CPACK_PACKAGE_URL "https://github.com/filesverse/filerix")
set(CPACK_PACKAGE_CONTACT "KingMaj0r <KingMaj0r@hotmail.com>")

set(CPACK_PACKAGE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_URL "https://github.com/filesverse/filerix")
set(CPACK_RPM_PACKAGE_DESCRIPTION "Filerix - File Management Library")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_RPM_PACKAGE_REQUIRES "zlib, libudev, dbus")
set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_SOURCE_DIR}/changelog-rpm")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
set(CPACK_DEBIAN_PACKAGE_CHANGELOG "${CMAKE_SOURCE_DIR}/changelog-deb")

if(WIN32)
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
