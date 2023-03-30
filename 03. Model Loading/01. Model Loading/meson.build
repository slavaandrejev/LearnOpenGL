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
  , include_directories: includeFolders
  , dependencies : [
        assimp_dep
      , boost_dep
      , epoxy_dep
      , fmt_dep
      , glm_dep
      , gtk_dep
      ]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
  )
