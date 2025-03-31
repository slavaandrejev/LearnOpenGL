gtk_files = gnome.compile_resources(
    'uniform-resources'
  , 'uniform.gresource.xml'
  , c_name: 'shaders_uniform'
)

executable(
    'shaders_uniform'
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
