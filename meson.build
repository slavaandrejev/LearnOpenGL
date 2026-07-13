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
if 'windows' == host_machine.system()
    # GObject Introspection errorneously includes `G_HAVE_GNUC_VISIBILITY` into
    # `glib-2.0.c`, while this macro is not available on Windows.
    add_project_arguments('-DG_HAVE_GNUC_VISIBILITY=0', language: ['c', 'cpp'])
    # It just removes the compiler error, but doesn't help code that relies on
    # `#if defined(G_HAVE_GNUC_VISIBILITY)`. In particular, it is used to define
    # `G_GNUC_INTERNAL`. Remove the correspondent warning.
    add_project_arguments('-Wno-attributes', language: ['c', 'cpp'])
endif

gnome = import('gnome')

assimp_dep    = dependency('assimp', method: 'cmake')
boost_dep     = dependency('boost', modules: ['filesystem'])
fmt_dep       = dependency('fmt', method: 'cmake')
glbinding_dep = dependency('glbinding', method: 'cmake')
glm_dep       = dependency('glm', method: 'cmake')

subdir('appwindow')
subdir('gi')

subdir('01. Getting started')
subdir('02. Lighting')
subdir('03. Model Loading')
