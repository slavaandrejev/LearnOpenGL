#include <glibmm/refptr.h>

#include "ex-004-window.h"

MainWindow::MainWindow(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::ApplicationWindow(cobject)
  , refBuilder(refBuilder)
{
    glArea = Gtk::Builder::get_widget_derived<OpenGLRender>(refBuilder, "glArea");
}

MainWindow* MainWindow::create() {
    auto refBuilder = Gtk::Builder::create_from_resource("/ex-004-window.ui");
    auto window     = Gtk::Builder::get_widget_derived<MainWindow>(refBuilder, "mainWindow");
    if (!window) {
        throw std::runtime_error(R"(No "mainWindow" object in ex-004-window.ui)");
    }
    return window;
}
