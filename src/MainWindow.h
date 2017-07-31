#pragma once
#include "Common.h"
#include "BackgroundManager.h"
#include "OutlinedLabel.h"
#include "ClockContainer.h"
#include <gtkmm.h>

namespace ClockPlus {

class MainWindow : public Gtk::ApplicationWindow {

private:
  class RandomClass {

  public:
    void set(const std::vector<std::string>& classes);
    const std::string* current();
    const std::string* next();

  private:
    void shuffle(const std::string* current);

  private:
    std::vector<std::string> m_classes;
    std::vector<size_t> m_order;

  };

private:
  static constexpr czstring CSS_FILE = "styles.css";
  static constexpr czstring TITLE = "Clock+";
  static constexpr czstring NAME = "MainWindow";
  static constexpr czstring NAME_TIME = "TimeLabel";
  static constexpr czstring NAME_DATE = "DateLabel";
  static constexpr czstring NAME_MESSAGE = "MessageLabel";
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 480;
  static constexpr int CLOCK_BORDER = 10;

public:
  MainWindow();
  ~MainWindow() override = default;

protected:
  // Bypass weak vtables warning
  void on_size_allocate(Gtk::Allocation& allocation) override;

private:
  void onDateChanged();
  void onMinuteChanged();
  void onCssChanged();
  void onBackgroundsChanged();
  std::string getBaseCss();
  static const std::string& getDefaultCss();
  void removeClass(const std::string* id);
  void addClass(const std::string* id);

private:
  Glib::RefPtr<Gtk::CssProvider> m_css;
  DateTimeProvider m_date_time;
  BackgroundManager m_backgrounds;
  std::unique_ptr<ClockContainer> m_clock;
  RandomClass m_class;

};

}