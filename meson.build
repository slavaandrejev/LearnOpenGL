project(
    'LearnOpenGL'
  , ['c', 'cpp'] # we need C to compile GTK resources
  , version : '0.1'
  , default_options: {
        'cpp_std': 'gnu++23'
      , 'c_std':   'gnu18'
      }
  )

compiler = meson.get_compiler('c')

project_folder = meson.current_source_dir()

include_folders = include_directories('include')

if get_option('buildtype') == ('debug')
    add_project_arguments('-DDEBUG', language: ['c', 'cpp'])
endif
if get_option('buildtype') == 'release' or get_option('buildtype') == 'debugoptimized'
    add_project_arguments('-DNDEBUG', language: ['c', 'cpp'])
endif
add_project_arguments('-DGLM_ENABLE_EXPERIMENTAL', language: 'cpp')

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
