include_directories(${CMAKE_SOURCE_DIR}/include)

if(TARGET_OS STREQUAL "Windows")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -ffunction-sections -fdata-sections")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -ffunction-sections -fdata-sections")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -shared -static-libgcc -static-libstdc++ -pthread -s -Wl,--gc-sections")

  set(SOURCES
    FileSystem/Windows/FileUtils.c
    Listeners/Windows/DriveListener.c
    Listeners/Windows/FileListener.c
    System/Windows/DriveUtils.c
    System/Windows/UserUtils.c
  )
else()
  set(SOURCES
    FileSystem/DirectoryPermissions.c
    FileSystem/FileUtils.c
    Listeners/DriveListener.c
    Listeners/FileListener.c
    System/DriveUtils.c
    System/UserUtils.c
  )

  include_directories(${UDEV_INCLUDE_DIRS})
endif()

set(OTHER_SOURCES
  FileSystem/FileUtils.cc
  Listeners/DriveListener.cc
  Listeners/FileListener.cc
  System/DriveUtils.cc
  System/UserUtils.cc

  Utils/Logger.c
)

set(LIBRARY_SOURCES ${SOURCES} ${OTHER_SOURCES})

add_library(filerix SHARED ${LIBRARY_SOURCES})

if(TARGET_OS STREQUAL "Windows")
  target_compile_definitions(filerix PRIVATE FILERIX_EXPORTS)
  target_link_libraries(filerix PRIVATE shlwapi ZLIB::ZLIB)
else()
  target_link_libraries(filerix PRIVATE ZLIB::ZLIB dbus-1 ${UDEV_LIBRARIES})
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ 
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/filerix
)

install(TARGETS filerix
  EXPORT filerixTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT filerixTargets
  FILE filerixConfig.cmake
  NAMESPACE filerix::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/filerix
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/filerixConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/filerixConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/filerix
)
