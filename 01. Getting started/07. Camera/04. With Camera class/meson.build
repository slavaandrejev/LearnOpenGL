gtk_files = gnome.compile_resources(
    'wcamera-resources'
  , 'wcamera.gresource.xml'
  , c_name: 'with_camera_wlz'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'wcamera'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
