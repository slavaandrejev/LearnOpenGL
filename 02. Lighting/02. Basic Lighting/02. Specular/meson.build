gtk_files = gnome.compile_resources(
    'specular-resources'
  , 'specular.gresource.xml'
  , c_name: 'specular'
)

executable(
    'specular'
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
