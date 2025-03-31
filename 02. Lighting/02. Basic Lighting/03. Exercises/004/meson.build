gtk_files = gnome.compile_resources(
    'ex-004-resources'
  , 'ex-004.gresource.xml'
  , c_name: 'ex_004'
)

executable(
    'ex-004'
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
