gio_dep      = dependency('gio-2.0')
glib_dep     = dependency('glib-2.0')
gmodule_dep  = dependency('gmodule-2.0')
gobject_dep  = dependency('gobject-2.0')

giglib_dep = [cppgir_dep, gio_dep, glib_dep, gmodule_dep, gobject_dep]
if 'windows' == host_machine.system()
    gio_windows_dep = dependency('gio-windows-2.0')
    giglib_dep += gio_windows_dep
elif 'linux' == host_machine.system()
    gio_unix_dep = dependency('gio-unix-2.0')
    giglib_dep += gio_unix_dep
endif

giglib_lib = static_library(
    'giglib'
  , gi_glib_gen
  , include_directories: gi_gen_inc
  , dependencies: giglib_dep
  )

giglib_dep = declare_dependency(
    sources: gi_glib_gen_dummy_h
  , link_with: giglib_lib
  , include_directories: gi_gen_inc
  , dependencies: giglib_dep
  )
