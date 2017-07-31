#include "Application.h"

namespace ClockPlus {

Application::AppRef Application::create(int narg, char** args) {
  AppRef app(new Application(narg, args));

  auto cancel = Gio::Cancellable::create();
  if(!app->register_application(cancel) || app->is_remote()) {
    return AppRef();
  }

  return app;
}

Application::Application(int narg, char** args)
: Gtk::Application(narg, args, ID) {}

void Application::on_activate() {
  m_main = std::make_unique<MainWindow>();
  add_window(*m_main);
  m_main->present();

  /*
  inhibit(
      *m_main,
      Gtk::ApplicationInhibitFlags::APPLICATION_INHIBIT_IDLE |
          Gtk::ApplicationInhibitFlags::APPLICATION_INHIBIT_SUSPEND,
      INHIBIT_REASON);
  */
}

}