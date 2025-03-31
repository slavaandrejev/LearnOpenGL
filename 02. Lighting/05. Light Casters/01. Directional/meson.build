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
  , dependencies : [
        boost_dep
      , fmt_dep
      , glbinding_dep
      , glm_dep
      , gtk_dep
      ]
  )
