#pragma once
#include "Common.h"
#include "Background/Backgrounds.h"
#include "Background/CssGenerator.h"
#include <functional>
#include <gtkmm.h>

namespace ClockPlus {

class BackgroundManager {

private:
  using IdVec = std::vector<std::string>;
  using Backgrounds = ClockPlus::Background::Backgrounds;
  using Period = ClockPlus::Background::Period;

private:
  static constexpr czstring JSON_FILE             = "backgrounds.json";
  static constexpr int      JSON_CHANGED_LIMIT_MS = 1000;

public:
  using CssHandler = std::function<void()>;
  using BackgroundsHandler = std::function<void()>;

public:
  explicit BackgroundManager(DateTimeProvider* date);
  void setCssHandler(CssHandler&& handler);
  void setBackgroundsHandler(BackgroundsHandler&& handler);
  Period::Minutes getPeriod() const noexcept;
  const std::string& getMessage() const noexcept;
  const IdVec& getBackgrounds() const noexcept;
  std::string getCss() const;
  void onDateChanged();

private:
  void notifyCss() const;
  void notifyBackgrounds() const;
  void onJsonChanged(
      const Glib::RefPtr<Gio::File>& file,
      const Glib::RefPtr<Gio::File>& other,
      Gio::FileMonitorEvent event);
  void load();

private:
  const DateTimeProvider* m_date;
  Glib::RefPtr<Gio::File>        m_json_file;
  Glib::RefPtr<Gio::FileMonitor> m_json_monitor;
  CssHandler                     m_handler_css;
  BackgroundsHandler             m_handler_back;
  Backgrounds                    m_backgrounds;
  std::string                    m_current_message;
  IdVec                          m_current_backgrounds;

};

}