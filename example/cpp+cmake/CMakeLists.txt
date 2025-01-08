cmake_minimum_required(VERSION 3.10)

project(example VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBFM REQUIRED libfm)

set(SOURCES
    src/example.cc
)

add_executable(example ${SOURCES})
target_link_libraries(example PRIVATE ${LIBFM_LIBRARIES})

target_include_directories(example PRIVATE ${LIBFM_INCLUDE_DIRS})
target_link_directories(example PRIVATE ${LIBFM_LIBRARY_DIRS})