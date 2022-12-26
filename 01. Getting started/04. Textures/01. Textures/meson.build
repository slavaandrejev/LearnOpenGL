gtk_files = gnome.compile_resources(
    'textures-resources'
  , 'textures.gresource.xml'
  , c_name: 'textures'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'textures'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
