gtk_files = gnome.compile_resources(
    'soft-spotlight-resources'
  , 'soft-spotlight.gresource.xml'
  , c_name: 'soft_spotlight'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'soft-spotlight'
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
