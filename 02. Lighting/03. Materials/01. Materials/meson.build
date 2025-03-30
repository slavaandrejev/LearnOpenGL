gtk_files = gnome.compile_resources(
    'materials-resources'
  , 'materials.gresource.xml'
  , c_name: 'materials'
)

executable(
    'materials'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
