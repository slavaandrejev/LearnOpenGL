#pragma once

#include <gtk/gtk.hpp>

#include <glbinding/glbinding.h>
#include <glbinding/getProcAddress.h>

#include <gnamespaces.h>

class GlBoundGlArea : public Gtk::impl::GLAreaImpl
{
    friend struct WidgetClassDef::TypeInitData;
public:
    GlBoundGlArea(Gtk::GLArea cobj, Gtk::Builder builder)
      : Gtk::impl::GLAreaImpl(cobj, this)
    {}

    GlBoundGlArea(const InitData &id, const char *type_name)
      : Gtk::impl::GLAreaImpl(this, id, type_name)
    {}

    GlBoundGlArea(const InitData &id)
      : GlBoundGlArea(id, "GlBoundGlArea")
    {}

protected:
    void realize_() noexcept override {
        static bool bound = false;

        Gtk::impl::GLAreaImpl::realize_();
        make_current();

        if (!bound) {
            glbinding::initialize(glbinding::getProcAddress, true);
            bound = true;
        }
    }
};
