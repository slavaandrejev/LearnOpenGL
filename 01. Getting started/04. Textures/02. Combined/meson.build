gtk_files = gnome.compile_resources(
    'combined-resources'
  , 'combined.gresource.xml'
  , c_name: 'combined'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'textures_combined'
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
