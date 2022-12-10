gtk_files = gnome.compile_resources(
    'cube-resources'
  , 'cube.gresource.xml'
  , c_name: 'cube'
  , source_dir: [project_folder / 'resources']
)

executable(
    'cube'
  , 'cube.cpp'
  , 'cube-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
