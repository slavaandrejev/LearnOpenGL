project(
    'LearnOpenGL'
  , ['c', 'cpp']
  , version : '0.1'
  , default_options: ['cpp_std=gnu++20', 'c_std=gnu2x'])

compiler = meson.get_compiler('c')

project_folder = meson.current_source_dir()

message('Build type: ' + get_option('buildtype'))
message('Project path: ' + project_folder)

includeFolders = include_directories('include')

compiler_definition = []
if get_option('buildtype') == ('debug')
    compiler_definition += ['-DDEBUG']
endif
if get_option('buildtype') == 'release' or get_option('buildtype') == 'debugoptimized'
    compiler_definition += ['-DNDEBUG']
endif


common_options = [
    '-march=native'
  , '-mtune=native'
  , '-fopenmp'
]

cpp_compiler_options = []

if compiler.get_id() == 'gcc'
    cpp_compiler_options += [
        '-Wno-class-memaccess'
      , '-Wno-deprecated-enum-enum-conversion'
    ]
    common_options += [
        '-Wno-deprecated-declarations'
      ]
    if get_option('buildtype').contains('debug')
        cpp_compiler_options += [
            '-fconcepts-diagnostics-depth=0'
          ]
    endif
    if get_option('buildtype') == 'release' or get_option('buildtype') == 'debugoptimized'
        common_options += [
            '-finline-limit=1000000', '-O3'
          ]
    endif
endif

if compiler.get_id().contains('clang')
    if get_option('buildtype').contains('debug')
        cpp_compiler_options += [
            '-ftemplate-backtrace-limit=0'
        ]
        common_options += [
            '-fno-limit-debug-info'
          ]
    endif
    if get_option('buildtype') == 'release' or get_option('buildtype') == 'debugoptimized'
        common_options += []
    endif
        cpp_compiler_options += [
            '-D_LIBCPP_ENABLE_CXX17_REMOVED_FEATURES'
        ]
        common_options += [
            '-Wno-unknown-cuda-version'
          , '-Wno-unsupported-target-opt'
          , '-Wno-unused-variable'
          , '-Wno-unused-value'
          , '-Wno-deprecated-builtins'
          , '-Wno-deprecated-declarations'
          , '-Wno-unused-but-set-variable'
          ]
    endif

if compiler.get_id() == 'gcc' or compiler.get_id().contains('clang')
    common_options += [
        '-ffunction-sections'
      , '-fdata-sections'
      , '-fPIC'
      ]

    if get_option('buildtype') == 'release' or get_option('buildtype') == 'debugoptimized'
        common_options += [
            '-flto'
          ]
    else
        common_options += []
    endif
    if get_option('buildtype').contains('debug')
        common_options += []
    endif
    if get_option('buildtype') == 'debugoptimized'
        common_options += [
            '-fno-omit-frame-pointer'
          , '-g3'
          ]
    endif
endif

c_compiler_options    = compiler_definition + common_options
cpp_compiler_options += compiler_definition + common_options
linker_options        = common_options + ['-Wl,--gc-sections', '-fopenmp']

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
glm_dep   = dependency('glm', method: 'cmake')
epoxy_dep = dependency('epoxy')
gtk_dep   = dependency('gtkmm-4.0')

subdir('01. Getting started')
subdir('02. Lighting')
