gtk_files = gnome.compile_resources(
    'cs-resources'
  , 'cs.gresource.xml'
  , c_name: 'cs'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'cs'
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
