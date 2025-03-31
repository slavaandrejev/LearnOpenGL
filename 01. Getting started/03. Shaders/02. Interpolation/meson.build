gtk_files = gnome.compile_resources(
    'interpolation-resources'
  , 'interpolation.gresource.xml'
  , c_name: 'shaders_interpolation'
)

executable(
    'shaders_interpolation'
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
