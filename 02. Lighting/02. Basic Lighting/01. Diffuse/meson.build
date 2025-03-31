gtk_files = gnome.compile_resources(
    'diffuse-resources'
  , 'diffuse.gresource.xml'
  , c_name: 'diffuse'
)

executable(
    'diffuse'
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
