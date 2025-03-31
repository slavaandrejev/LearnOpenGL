gtk_files = gnome.compile_resources(
    'point-resources'
  , 'point.gresource.xml'
  , c_name: 'point'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'point'
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
