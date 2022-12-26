gtk_files = gnome.compile_resources(
    'glm-resources'
  , 'glm.gresource.xml'
  , c_name: 'glm'
  , source_dir: [project_folder / 'resources' / 'textures']
)

executable(
    'glm'
  , 'glm.cpp'
  , 'glm-window.cpp'
  , 'render.cpp'
  , gtk_files
  , include_directories: includeFolders
  , dependencies : [fmt_dep, gtk_dep, epoxy_dep, boost_dep, glm_dep]
  , c_args: c_compiler_options
  , cpp_args: cpp_compiler_options
  , link_args: linker_options
)
