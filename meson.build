project(
    'LearnOpenGL'
  , ['c', 'cpp']
  , version : '0.1'
  , default_options: ['cpp_std=gnu++20', 'c_std=gnu2x'])

compiler = meson.get_compiler('c')

project_folder = meson.current_source_dir()

message('Build type: ' + get_option('buildcfg'))
message('Project path: ' + project_folder)

includeFolders = include_directories('include')

compiler_definition = []
if get_option('buildcfg') == ('debug')
    compiler_definition += ['-DDEBUG']
endif
if get_option('buildcfg') == 'release' or get_option('buildcfg') == 'debugoptimized'
    compiler_definition += ['-DNDEBUG']
endif

c_compiler_options   = []
cpp_compiler_options = []

common_options = [
    '-march=native'
  , '-mtune=native'
]

if compiler.get_id() == 'gcc'
    cpp_compiler_options += ['-Wno-class-memaccess', '-Wno-deprecated-enum-enum-conversion']
    common_options += [
        '-Wno-deprecated-declarations'
      ]
    if get_option('buildcfg') == 'release' or get_option('buildcfg') == 'debugoptimized'
        common_options += [
            '-finline-limit=1000000'
          ]
    endif
endif

if compiler.get_id().contains('clang')
    if get_option('buildcfg').contains('debug')
        common_options += [
            '-fno-limit-debug-info'
          ]
    endif
    if get_option('buildcfg') == 'release' or get_option('buildcfg') == 'debugoptimized'
        common_options += [
            '-mllvm --inline-threshold=8192'
          ]
    endif
endif

if compiler.get_id() == 'gcc' or compiler.get_id().contains('clang')
    common_options += [
        '-ffunction-sections'
      , '-fdata-sections'
      , '-fPIC'
      ]

    if get_option('buildcfg') == 'release' or get_option('buildcfg') == 'debugoptimized'
        common_options += [
            '-flto'
          , '-O3'
          ]
    else
        common_options += [
            '-O0'
          ]
    endif
    if get_option('buildcfg').contains('debug')
        common_options += [
            '-g3'
          ]
    endif
    if get_option('buildcfg') == 'debugoptimized'
        common_options += [
            '-fno-omit-frame-pointer'
          ]
    endif
endif

c_compiler_options   = compiler_definition + common_options
cpp_compiler_options = compiler_definition + common_options
linker_options = common_options + ['-Wl,--gc-sections']

if compiler.get_id().contains('clang')
    cpp_compiler_options += ['-stdlib=libc++']
    linker_options += [
        '-fuse-ld=lld'
      , '-stdlib=libc++'
      , '-Wl,--export-dynamic' # for dynamic_cast to work across module boundaries
      ]
endif

gnome = import('gnome')

boost_dep = dependency('boost')
fmt_dep   = dependency('fmt', method: 'cmake')
epoxy_dep = dependency('epoxy')
gtk_dep   = dependency('gtkmm-4.0')

subdir('01. Getting started/02. Hello Triangle/01. Hello Triangle')
subdir('01. Getting started/02. Hello Triangle/02. Hello Triangle (indexed)')
subdir('01. Getting started/02. Hello Triangle/03. Exercises/001')
subdir('01. Getting started/02. Hello Triangle/03. Exercises/002')
subdir('01. Getting started/02. Hello Triangle/03. Exercises/003')
subdir('01. Getting started/03. Shaders/01. Uniform')
subdir('01. Getting started/03. Shaders/02. Interpolation')
subdir('01. Getting started/03. Shaders/03. Exercises/001')
subdir('01. Getting started/03. Shaders/03. Exercises/002')
subdir('01. Getting started/03. Shaders/03. Exercises/003')
subdir('01. Getting started/04. Textures/01. Textures')
