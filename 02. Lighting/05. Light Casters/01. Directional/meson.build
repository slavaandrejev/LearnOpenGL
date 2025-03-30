gtk_files = gnome.compile_resources(
    'directional-resources'
  , 'directional.gresource.xml'
  , c_name: 'directional'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'directional'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
