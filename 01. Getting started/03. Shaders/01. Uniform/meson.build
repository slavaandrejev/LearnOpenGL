gtk_files = gnome.compile_resources(
    'uniform-resources'
  , 'uniform.gresource.xml'
  , c_name: 'shaders_uniform'
)

executable(
    'shaders_uniform'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
