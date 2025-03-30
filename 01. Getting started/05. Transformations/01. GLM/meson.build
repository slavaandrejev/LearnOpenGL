gtk_files = gnome.compile_resources(
    'glm-resources'
  , 'glm.gresource.xml'
  , c_name: 'glm'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'glm'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
