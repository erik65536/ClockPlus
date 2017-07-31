#include "BackgroundManager.h"
#include "Background/JsonLoader.h"
#include "Background/CssGenerator.h"

namespace ClockPlus {

using namespace Background;

void BackgroundManager::setCssHandler(CssHandler&& handler) {
  m_handler_css = std::move(handler);
}

void BackgroundManager::setBackgroundsHandler(BackgroundsHandler&& handler) {
  m_handler_back = std::move(handler);
}

void BackgroundManager::notifyCss() const {
  if(m_handler_css) {
    m_handler_css();
  }
}

void BackgroundManager::notifyBackgrounds() const {
  if(m_handler_back) {
    m_handler_back();
  }
}

Period::Minutes BackgroundManager::getPeriod() const noexcept {
  return m_backgrounds.getPeriod();
}

const std::string& BackgroundManager::getMessage() const noexcept {
  return m_current_message;
}

const BackgroundManager::IdVec& BackgroundManager::getBackgrounds(
    ) const noexcept {
  return m_current_backgrounds;
}

std::string BackgroundManager::getCss() const {
  return CssGenerator::generate(m_backgrounds);
}

BackgroundManager::BackgroundManager(DateTimeProvider* date)
: m_date{date} {
  m_json_file = Gio::File::create_for_parse_name(
      Glib::build_filename(getConfigDir(), JSON_FILE));

  load();

  m_json_monitor = m_json_file->monitor_file();
  m_json_monitor->set_rate_limit(JSON_CHANGED_LIMIT_MS);
  m_json_monitor->signal_changed().connect(
      sigc::mem_fun(*this, &BackgroundManager::onJsonChanged));
}

void BackgroundManager::onJsonChanged(
    const Glib::RefPtr<Gio::File>& file,
    const Glib::RefPtr<Gio::File>& other,
    Gio::FileMonitorEvent event) {
  (void)file;
  (void)other;
  (void)event;

  load();

  notifyCss();
}

void BackgroundManager::load() {
  try {
    std::string json_string = Glib::file_get_contents(m_json_file->get_path());
    JsonLoader::load(json_string, getConfigDir(), m_backgrounds);
  } catch(Glib::FileError& error) {
    // Load failed, set defaults.
    m_backgrounds.clear();
    if(error.code() != Glib::FileError::NO_SUCH_ENTITY) {
      g_warning("Error while reading backgrounds config file. Reason: %s",
          error.what().c_str());
    }
  } catch(std::exception& error) {
    // Load failed, set defaults.
    m_backgrounds.clear();
    g_warning("Error while parsing backgrounds config file. Reason: %s",
        error.what());
  }

  onDateChanged();
}

void BackgroundManager::onDateChanged() {
  if(!m_backgrounds.match(*m_date, m_current_message, m_current_backgrounds)) {
    notifyBackgrounds();
  }
}

}