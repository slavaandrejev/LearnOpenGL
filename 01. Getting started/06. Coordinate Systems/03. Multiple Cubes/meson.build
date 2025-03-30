gtk_files = gnome.compile_resources(
    'mcubes-resources'
  , 'mcubes.gresource.xml'
  , c_name: 'mcubes'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'mcubes'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
