cppgir = find_program('cppgir', required: false)
cppgir_ignore = []
if not cppgir.found()
    subproject('cppgir', default_options : ['internal_expected=false'])
    cppgir = find_program('cppgir')
    if 'windows' == host_machine.system()
        additional_ignores = configure_file(
            input: 'cppgir_win.ignore'
          , output: 'cppgir_win.ignore'
          , copy: true
          )
        cppgir_ignore = [
            '--ignore'
          , project_folder / 'subprojects' / 'cppgir' / 'data' / 'cppgir.ignore' + ';' +
            project_folder / 'subprojects' / 'cppgir' / 'data' / 'cppgir_win.ignore' + ';' +
            additional_ignores.full_path()
          ]
    elif 'linux' == host_machine.system()
        cppgir_ignore = [
            '--ignore'
          , project_folder / 'subprojects' / 'cppgir' / 'data' / 'cppgir.ignore' + ':' +
            project_folder / 'subprojects' / 'cppgir' / 'data' / 'cppgir_unix.ignore'
          ]
    endif
endif
if 'windows' == host_machine.system()
    pymod = import('python')
    python = pymod.find_installation()
    touch = [python, '-c', 'open("@OUTPUT@", "w").close()']
elif 'linux' == host_machine.system()
    touch = [find_program('touch'), '@OUTPUT@']
endif

cppgir_dep = dependency('cppgir')
gir_dir    = dependency('gobject-introspection-1.0').get_variable('girdir')

gi_glib_binding_names = [
    'gio'
  , 'glib'
  , 'gmodule'
  , 'gobject'
  ]
if 'windows' == host_machine.system()
    gi_glib_binding_names += ['giowin32', 'glibwin32']
elif 'linux' == host_machine.system()
    gi_glib_binding_names += ['giounix', 'glibunix']
endif

gi_gtk_binding_names = [
    'cairo'
  , 'freetype2'
  , 'gdk'
  , 'gdkpixbuf'
  , 'graphene'
  , 'gsk'
  , 'gtk'
  , 'harfbuzz'
  , 'pango'
  , 'pangocairo'
  ]

gi_glib_binding_src = []
foreach binding : gi_glib_binding_names
    gi_glib_binding_src += f'@binding@.cpp'
endforeach

gi_gtk_binding_src = []
foreach binding : gi_gtk_binding_names
    gi_gtk_binding_src += f'@binding@.cpp'
endforeach

gi_glib_modules = ['Gio-2.0', 'GLib-2.0', 'GModule-2.0', 'GObject-2.0']
if 'windows' == host_machine.system()
    gi_glib_modules += ['GLibWin32-2.0', 'GioWin32-2.0']
else
    gi_glib_modules += ['GLibUnix-2.0', 'GioUnix-2.0']
endif
gi_glib_gen_inputs = []
foreach module : gi_glib_modules
    gi_glib_gen_inputs += gir_dir / f'@module@.gir'
endforeach
if 'windows' == host_machine.system()
    gi_glib_gen_inputs += additional_ignores
endif
gi_glib_gen = custom_target('gi-glib-gen',
    input: gi_glib_gen_inputs
  , output: gi_glib_binding_src
  , command: [
        cppgir
      , '--output-top'
      , '--class'
      , '--output', '@OUTDIR@'
      ] + gi_glib_modules + cppgir_ignore
  )

gi_glib_gen_dummy_h = custom_target(
    'gi-glib-gen-dummy'
  , output: 'gi_glib_gen_dummy.h'
  , command: touch
  , input: gi_glib_gen
  )

gi_gtk_gen = custom_target('gi-gtk-gen',
    input: [
        gi_glib_gen_dummy_h
      , gir_dir / 'Gtk-4.0.gir'
      ]
  , output: gi_gtk_binding_src
  , command: [
        cppgir
      , '--output-top'
      , '--class'
      , '--only-changed'
      , '--output', '@OUTDIR@'
      , 'Gtk-4.0'
      ] + cppgir_ignore
  )

gi_gtk_gen_dummy_h = custom_target(
    'gi-gtk-gen-dummy'
  , output: 'gi_gtk_gen_dummy.h'
  , command: touch
  , input: gi_gtk_gen
  )

gi_gen_inc = include_directories('.')

subdir('giglib')
subdir('gigtk')
