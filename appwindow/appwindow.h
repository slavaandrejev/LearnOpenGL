#pragma once

#include <gtk/gtk.hpp>

#include <gnamespaces.h>

#include "render.h"

class MainWindow : public Gtk::impl::ApplicationWindowImpl
{
public:
    MainWindow(Gtk::ApplicationWindow cobj, Gtk::Builder builder);

    static gi::ref_ptr<MainWindow> new_();
};
