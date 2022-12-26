#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include <fmt/printf.h>

#include "diffuse-map-window.h"

int main (int argc, char *argv[]) {
    auto app = Gtk::Application::create();

    auto mainWindow = (MainWindow*)nullptr;
    app->signal_activate().connect([&]() {
        try {
            // The application has been started, so let's show a window.
            mainWindow = MainWindow::create();
            // Make sure that the application runs for as long this window is
            // still open.
            app->add_window(*mainWindow);
            mainWindow->present();
        }
        // If create_appwindow() throws an exception (perhaps from
        // Gtk::Builder), no window has been created, no window has been added
        // to the application, and therefore the application will stop running.
        catch (const Glib::Error& ex) {
            fmt::print(stderr, "Application::on_activate(): {}\n", ex.what());
        }
        catch (const std::exception& ex) {
            fmt::print(stderr, "Application::on_activate(): {}\n", ex.what());
        }
    });
    app->signal_window_removed().connect([&](Gtk::Window *window) {
        if (nullptr != mainWindow && dynamic_cast<Gtk::Window *>(mainWindow) == window) {
            delete mainWindow;
            mainWindow = nullptr;
        }
    });

    return app->run(argc, argv);
}
