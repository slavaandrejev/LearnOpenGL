gtk_files = gnome.compile_resources(
    'multiple-lights-resources'
  , 'multiple-lights.gresource.xml'
  , c_name: 'multiple_lights'
  , source_dir: [project_folder / 'resources' / 'textures']
  )

executable(
    'multiple-lights'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
