gtk_files = gnome.compile_resources(
    'ex-001-resources'
  , 'ex-001.gresource.xml'
  , c_name: 'shaders_ex_001'
)

executable(
    'ex-001'
  , 'ex-001.cpp'
  , 'ex-001-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
