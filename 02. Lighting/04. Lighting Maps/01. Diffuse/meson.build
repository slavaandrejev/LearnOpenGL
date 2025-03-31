gtk_files = gnome.compile_resources(
    'diffuse-map-resources'
  , 'diffuse-map.gresource.xml'
  , c_name: 'diffuse_map'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'diffuse-map'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [
        boost_dep
      , fmt_dep
      , glbinding_dep
      , glm_dep
      , gtk_dep
      ]
  )
