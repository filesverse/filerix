project('example', 'c', version: '1.0')

libfm = dependency('libfm', required: true)

src = files(
  'src/example.c',
)

executable('example', src, dependencies: libfm)