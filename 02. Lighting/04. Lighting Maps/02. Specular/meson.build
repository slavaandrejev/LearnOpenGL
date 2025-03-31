gtk_files = gnome.compile_resources(
    'specular-map-resources'
  , 'specular-map.gresource.xml'
  , c_name: 'specular_map'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'specular-map'
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
