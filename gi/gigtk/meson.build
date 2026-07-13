gtk_dep = dependency('gtk4')
if 'linux' == host_machine.system()
    # we need this dependency because of a cppgir defect
    gtk_print_dep = dependency('gtk4-unix-print')
endif

gigtk_deps = [cppgir_dep, giglib_dep, gtk_dep]
if 'linux' == host_machine.system()
    gigtk_deps += [gtk_print_dep]
endif

gigtk_lib = static_library(
    'gigtk'
  , gi_gtk_gen
  , include_directories: gi_gen_inc
  , dependencies: gigtk_deps
  )

gigtk_dep = declare_dependency(
    sources: gi_gtk_gen_dummy_h
  , link_with: gigtk_lib
  , dependencies: gigtk_deps
  )
