#include <gtk/gtk.hpp>

#include "appwindow.h"

MainWindow::MainWindow(Gtk::ApplicationWindow cobj, Gtk::Builder builder)
  : Gtk::impl::ApplicationWindowImpl(cobj, this)
{}

gi::ref_ptr<MainWindow> MainWindow::new_() {
    auto builder = Gtk::Builder::new_();
    gi::register_type<OpenGLRender>();
    if (builder.add_from_resource("/appwindow.ui")) {
        return builder.get_object_derived<MainWindow>("mainWindow");
    }
    // GTK has excellent error reporting infrastructure. There is no need to
    // check for errors here. GTK will throw an exception with the text
    // even sometimes translated to your language.
    return gi::ref_ptr<MainWindow>();
}
