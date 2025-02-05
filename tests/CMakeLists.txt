cmake_minimum_required(VERSION 3.14)
project(test_library VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FILERIX REQUIRED filerix)
set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(CRITERION REQUIRED criterion)

set(CXX_SOURCES
  src/TestUtils.cc
  src/FileSystem/FileUtils_Rename.cc
  src/FileSystem/FileUtils_GetFiles.cc
  src/FileSystem/FileUtils_SearchFiles.cc
  src/FileSystem/FileUtils_Copy.cc
  src/FileSystem/FileUtils_Cut.cc
  src/FileSystem/FileUtils_Compress.cc
  src/FileSystem/FileUtils_Decompress.cc
  src/FileSystem/UserUtils_ChangePermissions.cc
  src/FileSystem/UserUtils_GetUserName.cc
  src/FileSystem/DriveUtils_GetDrives.cc
  src/FileSystem/DriveUtils_GetDriveUsage.cc
  src/FileSystem/DriveUtils_Mount.cc
  src/FileSystem/DriveUtils_Unmount.cc
  src/FileSystem/DriveUtils_GetDeviceLabelOrUUID.cc
)

set(C_SOURCES
  src/TestUtils.c
  src/FileSystem/FileUtils_Rename.c
  #src/FileSystem/FileUtils_GetFiles.c
  #src/FileSystem/FileUtils_SearchFiles.c
  src/FileSystem/FileUtils_Copy.c
  src/FileSystem/FileUtils_Cut.c
  src/FileSystem/FileUtils_Compress.c
  src/FileSystem/FileUtils_Decompress.c
  src/FileSystem/UserUtils_ChangePermissions.c
  src/FileSystem/UserUtils_GetUserName.c
  src/FileSystem/DriveUtils_GetDrives.c
  src/FileSystem/DriveUtils_GetDriveUsage.c
  src/FileSystem/DriveUtils_Mount.c
  src/FileSystem/DriveUtils_Unmount.c
  src/FileSystem/DriveUtils_GetDeviceLabelOrUUID.c
)

add_executable(test_library_cxx ${CXX_SOURCES})
target_link_libraries(test_library_cxx PRIVATE ${FILERIX_LIBRARIES} ${CRITERION_LIBRARIES})
target_include_directories(test_library_cxx PRIVATE ${FILERIX_INCLUDE_DIRS} ${CRITERION_INCLUDE_DIRS} include)
target_link_directories(test_library_cxx PRIVATE ${FILERIX_LIBRARY_DIRS} ${CRITERION_LIBRARY_DIRS})

add_executable(test_library_c ${C_SOURCES})
target_link_libraries(test_library_c PRIVATE ${FILERIX_LIBRARIES} ${CRITERION_LIBRARIES})
target_include_directories(test_library_c PRIVATE ${FILERIX_INCLUDE_DIRS} ${CRITERION_INCLUDE_DIRS} include)
target_link_directories(test_library_c PRIVATE ${FILERIX_LIBRARY_DIRS} ${CRITERION_LIBRARY_DIRS})

enable_testing()
add_test(NAME test_library_cxx COMMAND test_library_cxx)
add_test(NAME test_library_c COMMAND test_library_c)
