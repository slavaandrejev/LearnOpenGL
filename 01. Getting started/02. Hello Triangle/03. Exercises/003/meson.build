gtk_files = gnome.compile_resources(
    'ex-003-resources'
  , 'ex-003.gresource.xml'
  , c_name: 'ex_003'
)

executable(
    'ex-003'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
