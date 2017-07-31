#include "Filter.h"

namespace ClockPlus::Background {

Filter::Filter(Priority priority)
: m_priority{priority} {}

Filter::Priority Filter::getPriority() const noexcept {
  return m_priority;
}

const std::string& Filter::getMessage() const noexcept {
  return m_message;
}

const Filter::BackgroundVec& Filter::getBackgrounds() const noexcept {
  return m_backgrounds;
}

void Filter::setMessage(const std::string& message) {
  m_message = message;
}

void Filter::addBackground(size_t background) {
  m_backgrounds.emplace_back(background);
}

Date& Filter::addDate() {
  m_dates.emplace_back();
  return m_dates.back();
}

bool Filter::isToday(const DateTimeProvider& date) const {
  if(m_dates.empty()) {
    return true;
  }

  for(const auto& i : m_dates) {
    if(i.isToday(date)) {
      return true;
    }
  }

  return false;
}

}