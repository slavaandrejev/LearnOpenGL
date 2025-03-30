gtk_files = gnome.compile_resources(
    'point-resources'
  , 'point.gresource.xml'
  , c_name: 'point'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'point'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
