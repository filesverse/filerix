project('fm', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++17']
)

library_sources = files(
  'src/FileSystem/DirectoryPermissions.cc',
  'src/FileSystem/FileUtils.cc',
  'src/FileSystem/MountUtils.cc',
  'src/FileSystem/UserUtils.cc'
)

libfm = library(
  'fm',
  library_sources,
  include_directories: include_directories('src', 'include'),
  install: true
)

install_subdir('include', install_dir: join_paths(get_option('prefix'), 'include', 'libfm'), strip_directory: true)

install_data('libfm.pc', install_dir: join_paths(get_option('prefix'), 'lib/pkgconfig'))