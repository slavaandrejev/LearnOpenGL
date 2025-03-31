gtk_files = gnome.compile_resources(
    'model-loading-resources'
  , 'model-loading.gresource.xml'
  , c_name: 'model_loading'
  , source_dir: [project_folder]
  )

executable(
    'model-loading'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [
        assimp_dep
      , boost_dep
      , fmt_dep
      , glbinding_dep
      , glm_dep
      , gtk_dep
      ]
  )
