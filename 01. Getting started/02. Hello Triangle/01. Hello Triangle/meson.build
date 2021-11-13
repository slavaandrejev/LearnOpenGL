gtk_files = gnome.compile_resources(
    'hello-triangle-resources'
  , 'hello-triangle.gresource.xml'
  , c_name: 'hello_triangle'
)

executable(
    'hello-triangle'
  , 'hello-triangle.cpp'
  , 'hello-triangle-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
