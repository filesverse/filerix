include_directories(${CMAKE_SOURCE_DIR}/include ${UDEV_INCLUDE_DIRS})

set(LIBRARY_SOURCES
  FileSystem/C/DriveUtils.cc
  FileSystem/C/FileUtils.cc
  FileSystem/C/UserUtils.cc
  FileSystem/DirectoryPermissions.cc
  FileSystem/DriveUtils.cc
  FileSystem/FileUtils.cc
  FileSystem/UserUtils.cc
  Utils/Logger.cc
  Listeners/DriveListener.cc
  Listeners/FileListener.cc
  Listeners/C/DriveListener.cc
  Listeners/C/FileListener.cc
)

add_library(filerix SHARED ${LIBRARY_SOURCES})

target_link_libraries(filerix PRIVATE ZLIB::ZLIB ${UDEV_LIBRARIES})

install(TARGETS filerix
  EXPORT filerixTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/filerix)

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
