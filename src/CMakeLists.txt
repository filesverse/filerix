include_directories(${CMAKE_SOURCE_DIR}/include)

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

target_link_libraries(filerix PRIVATE ZLIB::ZLIB)

install(TARGETS filerix
  EXPORT filerixTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include/filerix)

install(EXPORT filerixTargets
  FILE filerixConfig.cmake
  NAMESPACE filerix::
  DESTINATION share/filerix
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/filerixConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/filerixConfigVersion.cmake"
  DESTINATION share/filerix
)
