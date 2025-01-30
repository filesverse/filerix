project('fm', ['cpp', 'c'], version: '1.0', default_options: ['cpp_std=c++17'])

library_sources = files(
  'src/FileSystem/DirectoryPermissions.cc',
  'src/FileSystem/DriveUtils.c',
  'src/FileSystem/DriveUtils.cc',
  'src/FileSystem/FileUtils.cc',
  'src/FileSystem/UserUtils.cc',
  'src/Utils/Logger.c',
  'src/Utils/Logger.cc',
)

libfm = library(
  'fm',
  library_sources,
  include_directories: include_directories('src', 'include'),
  install: true,
)

install_subdir(
  'include',
  install_dir: join_paths(get_option('prefix'), 'include', 'libfm'),
  strip_directory: true,
)

install_subdir(
  'types',
  install_dir: join_paths(get_option('prefix'), 'lib', 'libfm'),
  strip_directory: true,
)

pkgconfig = import('pkgconfig')

pkgconfig.generate(
  name: 'libfm',
  description: 'A file manager library',
  version: meson.project_version(),
  libraries: libfm,
  subdirs: ['include/libfm'],
  filebase: 'libfm',
)