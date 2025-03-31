gtk_files = gnome.compile_resources(
    'multiple-lights-resources'
  , 'multiple-lights.gresource.xml'
  , c_name: 'multiple_lights'
  , source_dir: [project_folder / 'resources' / 'textures']
  )

executable(
    'multiple-lights'
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
