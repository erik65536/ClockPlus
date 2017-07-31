#pragma once
#include "Common.h"
#include "MainWindow.h"

namespace ClockPlus {

class Application : public Gtk::Application {

public:
  using AppRef = Glib::RefPtr<Application>;

private:
  static constexpr czstring ID = "org.ErikKessler.ClockPlus";
  static constexpr czstring INHIBIT_REASON = "The screen is not allowed to "
      "sleep while the clock is running.";

public:
  static AppRef create(int narg, char** args);

protected:
  Application(int narg, char** args);
  void on_activate() override;

private:
  std::unique_ptr<MainWindow> m_main;

};

}
