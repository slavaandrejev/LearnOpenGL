gtk_files = gnome.compile_resources(
    'wlz-resources'
  , 'wlz.gresource.xml'
  , c_name: 'camera_wlz'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'wlz'
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
