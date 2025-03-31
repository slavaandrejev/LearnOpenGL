gtk_files = gnome.compile_resources(
    'cube-resources'
  , 'cube.gresource.xml'
  , c_name: 'cube'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'cube'
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
