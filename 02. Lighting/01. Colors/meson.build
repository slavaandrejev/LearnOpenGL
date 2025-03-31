gtk_files = gnome.compile_resources(
    'colors-resources'
  , 'colors.gresource.xml'
  , c_name: 'colors'
)

executable(
    'colors'
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
