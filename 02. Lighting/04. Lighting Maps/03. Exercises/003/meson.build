gtk_files = gnome.compile_resources(
    'ex-003-resources'
  , 'ex-003.gresource.xml'
  , c_name: 'ex_003'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'ex-003'
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
