gtk_files = gnome.compile_resources(
    'materials-resources'
  , 'materials.gresource.xml'
  , c_name: 'materials'
)

executable(
    'materials'
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
