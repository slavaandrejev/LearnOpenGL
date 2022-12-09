gtk_files = gnome.compile_resources(
    'ex-003-resources'
  , 'ex-003.gresource.xml'
  , c_name: 'textures_ex_003'
  , source_dir: [project_folder / 'resources']
)

executable(
    'ex-003'
  , 'ex-003.cpp'
  , 'ex-003-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
