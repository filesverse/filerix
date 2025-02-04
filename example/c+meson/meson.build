project('example', 'c', version: '1.0')

filerix = dependency('filerix', required: true)

src = files(
  'src/example.c',
)

executable('example', src, dependencies: filerix)