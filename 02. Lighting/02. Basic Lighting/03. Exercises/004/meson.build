gtk_files = gnome.compile_resources(
    'ex-004-resources'
  , 'ex-004.gresource.xml'
  , c_name: 'ex_004'
)

executable(
    'ex-004'
  , 'ex-004.cpp'
  , 'ex-004-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
