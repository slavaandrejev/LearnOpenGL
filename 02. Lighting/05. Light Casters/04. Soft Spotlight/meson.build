gtk_files = gnome.compile_resources(
    'soft-spotlight-resources'
  , 'soft-spotlight.gresource.xml'
  , c_name: 'soft_spotlight'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'soft-spotlight'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
