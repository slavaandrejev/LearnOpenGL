gtk_files = gnome.compile_resources(
    'spotlight-resources'
  , 'spotlight.gresource.xml'
  , c_name: 'spotlight'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'spotlight'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
