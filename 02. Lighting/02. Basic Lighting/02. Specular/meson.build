gtk_files = gnome.compile_resources(
    'specular-resources'
  , 'specular.gresource.xml'
  , c_name: 'specular'
)

executable(
    'specular'
  , appwindow
  , 'render.cpp'
  , gtk_files
  , include_directories: include_folders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  )
