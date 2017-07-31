#include "Date.h"

namespace ClockPlus::Background {

void Date::addTag(const std::string& tag) {
  m_tags.push_back(tag);
}

void Date::addYear(int year) {
  m_years.push_back(year);
}

void Date::addMonth(int month) {
  m_months.push_back(month);
}

void Date::addDayOfMonth(int day_of_month) {
  m_days_of_month.push_back(day_of_month);
}

void Date::addDayOfWeek(int day_of_week) {
  m_days_of_week.push_back(day_of_week);
}

bool Date::isToday(const DateTimeProvider& date) const {
  return matchTags(date) &&
      match(m_years, date.getYear()) &&
      match(m_months, date.getMonth()) &&
      match(m_days_of_month, date.getDayOfMonth()) &&
      match(m_days_of_week, date.getDayOfWeek());
}

bool Date::matchTags(const DateTimeProvider& date) const {
  if(m_tags.empty()) {
    return true;
  }

  for(const auto& tag : m_tags) {
    if(date.hasTag(tag)) {
      return true;
    }
  }

  return false;
}

bool Date::match(const std::vector<int>& list, int match) {
  if(list.empty()) {
    return true;
  }

  for(auto item : list) {
    if(item == match) {
      return true;
    }
  }

  return false;
}

}