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
  , include_directories: include_folders
  , dependencies : [
        boost_dep
      , fmt_dep
      , glbinding_dep
      , gtk_dep
      ]
  )
