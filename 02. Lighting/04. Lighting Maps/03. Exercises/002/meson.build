gtk_files = gnome.compile_resources(
    'ex-002-resources'
  , 'ex-002.gresource.xml'
  , c_name: 'ex_002'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'ex-002'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
