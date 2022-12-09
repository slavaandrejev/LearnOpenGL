#include <glibmm/refptr.h>

#include "glm-window.h"

MainWindow::MainWindow(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::ApplicationWindow(cobject)
  , refBuilder(refBuilder)
  , keyEvents(Gtk::EventControllerKey::create())
{
    glArea = Gtk::Builder::get_widget_derived<OpenGLRender>(refBuilder, "glArea");

    add_controller(keyEvents);
    keyEvents->signal_key_pressed().connect([&](guint, guint, Gdk::ModifierType) {
        keyEvents->forward(*glArea);
        return true;
    }, true);
}

MainWindow* MainWindow::create() {
    auto refBuilder = Gtk::Builder::create_from_resource("/glm-window.ui");
    auto window     = Gtk::Builder::get_widget_derived<MainWindow>(refBuilder, "mainWindow");
    if (!window) {
        throw std::runtime_error(R"(No "mainWindow" object in glm-window.ui)");
    }
    return window;
}
