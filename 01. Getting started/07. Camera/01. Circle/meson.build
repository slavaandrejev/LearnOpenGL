gtk_files = gnome.compile_resources(
    'circle-resources'
  , 'circle.gresource.xml'
  , c_name: 'camera_circle'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'circle'
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
