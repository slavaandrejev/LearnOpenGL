project(
    'LearnOpenGL'
  , ['c', 'cpp'] # we need C to compile GTK resources
  , version : '0.1'
  , default_options: ['cpp_std=gnu++20', 'c_std=gnu2x'])

compiler = meson.get_compiler('c')

project_folder = meson.current_source_dir()

includeFolders = include_directories('include')

compiler_definition = []
cpp_compiler_options = []
c_compiler_options = []
common_options = []
if get_option('buildtype') == ('debug')
    compiler_definition += ['-DDEBUG']
else
    compiler_definition += ['-DNDEBUG']
    common_options = [
        '-march=native'
      , '-mtune=native'
      ]
endif

c_compiler_options   += compiler_definition + common_options
cpp_compiler_options += compiler_definition + common_options
linker_options        = common_options + []

gnome = import('gnome')

boost_dep = dependency('boost')
fmt_dep   = dependency('fmt', method: 'cmake')
glm_dep   = dependency('glm', method: 'cmake')
epoxy_dep = dependency('epoxy')
gtk_dep   = dependency('gtkmm-4.0')

subdir('01. Getting started')
subdir('02. Lighting')
