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

assimp_dep = dependency('assimp', method: 'cmake')
boost_dep  = dependency('boost', modules: ['filesystem'])
epoxy_dep  = dependency('epoxy')
fmt_dep    = dependency('fmt', method: 'cmake')
glm_dep    = dependency('glm', method: 'cmake')
gtk_dep    = dependency('gtkmm-4.0')

subdir('appwindow')

subdir('01. Getting started')
subdir('02. Lighting')
subdir('03. Model Loading')
