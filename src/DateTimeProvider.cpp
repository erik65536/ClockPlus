#include "DateTimeProvider.h"

namespace ClockPlus {

DateTimeProvider::DateTimeProvider() {
  m_time = Glib::DateTime::create_now_local();
  m_year = m_time.get_year();
  m_month = m_time.get_month();
  m_day_of_month = m_time.get_day_of_month();
  m_hour = m_time.get_hour();
  m_minute = m_time.get_minute();

  setTimer();
}

void DateTimeProvider::setDateHandler(DateHandler&& handler) {
  m_handler_date = std::move(handler);
}

void DateTimeProvider::setMinuteHandler(MinuteHandler&& handler) {
  m_handler_minute = std::move(handler);
}

void DateTimeProvider::notifyDateChanged() {
  if(m_handler_date) {
    m_handler_date();
  }
}

void DateTimeProvider::notifyMinuteChanged() {
  if(m_handler_minute) {
    m_handler_minute();
  }
}

void DateTimeProvider::onUpdate() {
  m_time = Glib::DateTime::create_now_local();

  int year = m_time.get_year();
  int month = m_time.get_month();
  int day_of_month = m_time.get_day_of_month();
  int hour = m_time.get_hour();
  int minute = m_time.get_minute();

  bool date_changed = year != m_year ||
      month != m_month ||
      day_of_month != m_day_of_month;

  if(date_changed) {
    m_year = year;
    m_month = month;
    m_day_of_month = day_of_month;
  }

  bool minute_changed = hour != m_hour || minute != m_minute;

  if(minute_changed) {
    m_hour = hour;
    m_minute = minute;
  }

  if(date_changed) {
    notifyDateChanged();
  }
  if(minute_changed) {
    notifyMinuteChanged();
  }

  setTimer();
}

void DateTimeProvider::setTimer() {
  auto sec = static_cast<unsigned int>(m_time.get_second());
  auto usec = static_cast<unsigned int>(m_time.get_microsecond());

  // This is a leap second.
  if(sec == SEC_PER_MIN) {
    --sec;
  }

  unsigned int next = MS_PER_MIN - (sec*MS_PER_SEC+usec/USEC_PER_MS) + MS_FUDGE;

  Glib::signal_timeout().connect_once(
      sigc::mem_fun(*this, &DateTimeProvider::onUpdate),
      next);
}

int DateTimeProvider::getYear() const noexcept {
  return m_year;
}

int DateTimeProvider::getYearTwoDigit() const noexcept {
  return getYear() % 100;
}

int DateTimeProvider::getMonth() const noexcept {
  return m_month;
}

czstring DateTimeProvider::getMonthShort() const noexcept {
  return lookupName(getMonth()-1, MONTH_SHORT, MONTH_NONE_SHORT);
}

czstring DateTimeProvider::getMonthLong() const noexcept {
  return lookupName(getMonth()-1, MONTH_LONG, MONTH_NONE_LONG);
}

int DateTimeProvider::getDayOfMonth() const noexcept {
  return m_day_of_month;
}

int DateTimeProvider::getDayOfWeek() const noexcept {
  return m_time.get_day_of_week();
}

czstring DateTimeProvider::getDayOfWeekShort() const noexcept {
  return lookupName(
      getDayOfWeek()-1,
      DAY_OF_WEEK_SHORT,
      DAY_OF_WEEK_NONE_SHORT);
}

czstring DateTimeProvider::getDayOfWeekLong() const noexcept {
  return lookupName(
      getDayOfWeek()-1,
      DAY_OF_WEEK_LONG,
      DAY_OF_WEEK_NONE_LONG);
}

int DateTimeProvider::getHour() const noexcept {
  return m_hour;
}

int DateTimeProvider::getHour12h() const noexcept {
  int hour = getHour();

  if(hour >= 12) {
    hour -= 12;
  }
  return hour == 0 ? 12 : hour;
}

int DateTimeProvider::getMinute() const noexcept {
  return m_minute;
}

bool DateTimeProvider::hasTag(const std::string& tag) const {
  (void)tag;
  return false;
}

}