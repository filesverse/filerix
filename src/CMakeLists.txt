include_directories(${NLOHMANN_JSON_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

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

target_link_libraries(filerix PRIVATE ZLIB::ZLIB nlohmann_json::nlohmann_json)

install(TARGETS filerix
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include/filerix)
