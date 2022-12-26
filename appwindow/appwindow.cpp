#include <glibmm/refptr.h>

#include "appwindow.h"

MainWindow::MainWindow(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::ApplicationWindow(cobject)
  , refBuilder(refBuilder)
{
    glArea = Gtk::Builder::get_widget_derived<OpenGLRender>(refBuilder, "glArea");
}

MainWindow* MainWindow::create() {
    auto refBuilder = Gtk::Builder::create_from_resource("/appwindow.ui");
    auto window     = Gtk::Builder::get_widget_derived<MainWindow>(refBuilder, "mainWindow");
    // GTK has excellent error reporting infrastructure. There is no need to
    // check for errors here. GTK will throw an exception with the text
    // even sometimes translated to your language.
    return window;
}
