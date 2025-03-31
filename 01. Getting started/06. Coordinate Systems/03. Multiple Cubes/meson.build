gtk_files = gnome.compile_resources(
    'mcubes-resources'
  , 'mcubes.gresource.xml'
  , c_name: 'mcubes'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'mcubes'
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
