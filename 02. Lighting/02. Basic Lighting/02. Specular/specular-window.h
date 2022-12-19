#pragma once

#include <glibmm/refptr.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>

#include "render.h"

class MainWindow : public Gtk::ApplicationWindow
{
public:
    MainWindow(BaseObjectType* cobject,
               const Glib::RefPtr<Gtk::Builder>& refBuilder);

    static MainWindow* create();

private:
    Glib::RefPtr<Gtk::Builder> refBuilder;
    OpenGLRender *glArea = nullptr;
};
