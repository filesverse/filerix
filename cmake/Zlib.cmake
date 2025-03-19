if(WIN32)
  find_package(ZLIB REQUIRED)

  get_filename_component(ZLIB_LIB_DIR "${ZLIB_LIBRARY_RELEASE}" DIRECTORY)
  set(ZLIB_DLL_PATH "${ZLIB_LIB_DIR}/../bin/zlib1.dll")

  if(EXISTS "${ZLIB_DLL_PATH}")
    install(FILES ${ZLIB_DLL_PATH} DESTINATION ${CMAKE_INSTALL_BINDIR})
  else()
    message(WARNING "zlib1.dll not found! Make sure zlib is installed via vcpkg.")
  endif()
endif()