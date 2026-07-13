#include <gtk/gtk.hpp>

#include <fmt/printf.h>

#include <gnamespaces.h>

#include "appwindow.h"

int main (int argc, char *argv[]) {
    auto app = Gtk::Application::new_(Gio::ApplicationFlags::DEFAULT_FLAGS_);

    auto main_window = gi::ref_ptr<MainWindow>{};
    app.signal_activate().connect([&](Gio::Application app_) {
        try {
            // The application has been started, so let's show a window.
            main_window = MainWindow::new_();
            // Make sure that the application runs for as long this window is
            // still open.
            app.add_window(*main_window);
            main_window->present();
        }
        // If create_appwindow() throws an exception (perhaps from
        // Gtk::Builder), no window has been created, no window has been added
        // to the application, and therefore the application will stop running.
        catch (const GLib::Error& ex) {
            fmt::print(stderr, "Application::on_activate(): {}\n", ex.what());
            app_.quit();
        }
        catch (const std::exception& ex) {
            fmt::print(stderr, "Application::on_activate(): {}\n", ex.what());
            app_.quit();
        }
    });
    app.signal_window_removed().connect([&](Gtk::Application, Gtk::Window window) {
        if (main_window && *main_window == window) {
            main_window.reset();
        }
    });

    return app.run({argv, size_t(argc)});
}
