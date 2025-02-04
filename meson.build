project('filerix', ['cpp', 'c'], version: '1.0', default_options: ['cpp_std=c++17'])

dependencies = [
  dependency('libudev', required: true)
]

library_sources = files(
  'src/FileSystem/C/DriveUtils.cc',
  'src/FileSystem/C/UserUtils.cc',
  'src/FileSystem/DirectoryPermissions.cc',
  'src/FileSystem/DriveUtils.cc',
  'src/FileSystem/FileUtils.cc',
  'src/FileSystem/UserUtils.cc',
  'src/Utils/Logger.cc',
  'src/Listeners/DriveListener.cc',
  'src/Listeners/FileListener.cc',
  'src/Listeners/C/DriveListener.cc',
  'src/Listeners/C/FileListener.cc',
)

filerix = library(
  'filerix',
  library_sources,
  include_directories: include_directories('src', 'include'),
  dependencies: dependencies,
  install: true,
)

install_subdir(
  'include',
  install_dir: join_paths(get_option('prefix'), 'include', 'filerix'),
  strip_directory: true,
)

install_subdir(
  'types',
  install_dir: join_paths(get_option('prefix'), 'lib', 'filerix'),
  strip_directory: true,
)

pkgconfig = import('pkgconfig')

pkgconfig.generate(
  name: 'filerix',
  description: 'A file manager library',
  version: meson.project_version(),
  libraries: filerix,
  subdirs: ['include/filerix'],
  filebase: 'filerix',
)