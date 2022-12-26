gtk_files = gnome.compile_resources(
    'diffuse-map-resources'
  , 'diffuse-map.gresource.xml'
  , c_name: 'diffuse_map'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'diffuse-map'
  , 'diffuse-map.cpp'
  , 'diffuse-map-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
