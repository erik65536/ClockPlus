#pragma once
#include "Common.h"
#include <string>
#include <array>
#include <giomm.h>

namespace ClockPlus {

class DateTimeProvider {

private:
  static constexpr unsigned int SEC_PER_MIN = 60;
  static constexpr unsigned int MS_PER_SEC = 1000;
  static constexpr unsigned int USEC_PER_MS = 1000;
  static constexpr unsigned int MS_PER_MIN = SEC_PER_MIN*MS_PER_SEC;
  static constexpr unsigned int USEC_PER_SEC = 10000000;
  static constexpr unsigned int MS_FUDGE = 10;
  static constexpr std::array<czstring, 7> DAY_OF_WEEK_SHORT = {{
      "Mon","Tue","Wed","Thu","Fri","Sat","Sun"}};
  static constexpr std::array<czstring, 7> DAY_OF_WEEK_LONG = {{
      "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"}};
  static constexpr czstring DAY_OF_WEEK_NONE_SHORT = "Err";
  static constexpr czstring DAY_OF_WEEK_NONE_LONG = "Error";
  static constexpr std::array<czstring, 12> MONTH_SHORT = {{
      "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}};
  static constexpr std::array<czstring, 12> MONTH_LONG = {{
      "January","February","Match","April","May","June","July","August",
      "September","October","November","December"}};
  static constexpr czstring MONTH_NONE_SHORT = "Err";
  static constexpr czstring MONTH_NONE_LONG = "Error";

public:
  using DateHandler = std::function<void()>;
  using MinuteHandler = std::function<void()>;

public:
  DateTimeProvider();
  void setDateHandler(DateHandler&& handler);
  void setMinuteHandler(MinuteHandler&& handler);
  int getYear() const noexcept;
  int getYearTwoDigit() const noexcept;
  int getMonth() const noexcept;
  czstring getMonthShort() const noexcept;
  czstring getMonthLong() const noexcept;
  int getDayOfMonth() const noexcept;
  int getDayOfWeek() const noexcept;
  czstring getDayOfWeekShort() const noexcept;
  czstring getDayOfWeekLong() const noexcept;
  int getHour() const noexcept;
  int getHour12h() const noexcept;
  int getMinute() const noexcept;
  bool hasTag(const std::string& tag) const;

private:
  void setTimer();
  void onUpdate();
  void notifyDateChanged();
  void notifyMinuteChanged();

  template<size_t N>
  static czstring lookupName(
      int val,
      const std::array<czstring, N>& array,
      czstring none) {
    if(val < 0 || static_cast<size_t>(val) > array.size()) {
      return none;
    }
    return array[static_cast<size_t>(val)];
  }

private:
  Glib::DateTime m_time;
  int m_year;
  int m_month;
  int m_day_of_month;
  int m_hour;
  int m_minute;
  DateHandler m_handler_date;
  MinuteHandler m_handler_minute;

};

}