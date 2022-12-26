gtk_files = gnome.compile_resources(
    'ex-002-resources'
  , 'ex-002.gresource.xml'
  , c_name: 'ex_002'
)

executable(
    'ex-002'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
