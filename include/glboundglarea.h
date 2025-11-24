#pragma once

#include <gtkmm/glarea.h>

#include <glbinding/glbinding.h>
#include <glbinding/getProcAddress.h>

class GlBoundGlArea : public Gtk::GLArea
{
public:
    GlBoundGlArea(BaseObjectType *cobject)
      : Gtk::GLArea(cobject)
    {}

protected:
    void on_realize() override {
        static bool bound = false;

        Gtk::GLArea::on_realize();
        make_current();

        if (!bound) {
            glbinding::initialize(glbinding::getProcAddress, true);
            bound = true;
        }
    }
};
