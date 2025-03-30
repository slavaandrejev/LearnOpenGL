gtk_files = gnome.compile_resources(
    'ex-001-resources'
  , 'ex-001.gresource.xml'
  , c_name: 'textures_ex_001'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'ex-001'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep]
  )
