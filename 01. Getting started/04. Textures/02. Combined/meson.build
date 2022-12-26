gtk_files = gnome.compile_resources(
    'combined-resources'
  , 'combined.gresource.xml'
  , c_name: 'combined'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'textures_combined'
  , 'combined.cpp'
  , 'combined-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
