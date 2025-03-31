gtk_files = gnome.compile_resources(
    'spotlight-resources'
  , 'spotlight.gresource.xml'
  , c_name: 'spotlight'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'spotlight'
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
