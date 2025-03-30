gtk_files = gnome.compile_resources(
    'combined-resources'
  , 'combined.gresource.xml'
  , c_name: 'combined'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'textures_combined'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep]
  )
