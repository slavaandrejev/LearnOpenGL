gtk_files = gnome.compile_resources(
    'wcamera-resources'
  , 'wcamera.gresource.xml'
  , c_name: 'with_camera_wlz'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'wcamera'
  , 'wcamera.cpp'
  , 'wcamera-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
