#pragma once
#include "Background.h"
#include "Date.h"
#include <cstdint>
#include <string>
#include <vector>

namespace ClockPlus::Background {

class Filter {

public:
  using Priority = uint32_t;
  using BackgroundVec = std::vector<size_t>;
  using DateVec = std::vector<Date>;
  static constexpr Priority DEFAULT_PRIORITY = 0;

public:
  explicit Filter(Priority priority);
  Priority getPriority() const noexcept;
  const std::string& getMessage() const noexcept;
  void setMessage(const std::string& message);
  const BackgroundVec& getBackgrounds() const noexcept;
  void addBackground(size_t background);
  Date& addDate();
  bool isToday(const DateTimeProvider& date) const;

private:
  const Priority m_priority;
  std::string    m_message;
  BackgroundVec  m_backgrounds;
  DateVec        m_dates;

};

}