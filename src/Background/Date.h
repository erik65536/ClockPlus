#pragma once
#include "../DateTimeProvider.h"
#include <string>
#include <vector>

namespace ClockPlus::Background {

class Date {

public:
  bool isToday(const DateTimeProvider& date) const;
  void addTag(const std::string& tag);
  void addYear(int year);
  void addMonth(int month);
  void addDayOfMonth(int day_of_month);
  void addDayOfWeek(int day_of_week);

private:
  bool matchTags(const DateTimeProvider& date) const;
  static bool match(const std::vector<int>& list, int match);

private:
  std::vector<std::string> m_tags;
  std::vector<int> m_years;
  std::vector<int> m_months;
  std::vector<int> m_days_of_month;
  std::vector<int> m_days_of_week;

};

}