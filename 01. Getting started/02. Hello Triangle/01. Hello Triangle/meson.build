gtk_files = gnome.compile_resources(
    'hello-triangle-resources'
  , 'hello-triangle.gresource.xml'
  , c_name: 'hello_triangle'
)

executable(
    'hello-triangle'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
