gtk_files = gnome.compile_resources(
    'interpolation-resources'
  , 'interpolation.gresource.xml'
  , c_name: 'shaders_interpolation'
)

executable(
    'shaders_interpolation'
  , 'interpolation.cpp'
  , 'interpolation-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
