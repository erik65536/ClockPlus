#pragma once

namespace ClockPlus::Background {

class Period {

public:
  using Minutes = int;

private:
  static constexpr Minutes MIN = 1;
  static constexpr Minutes DEFAULT = 60;

public:
  void set(Minutes period);
  void setDefault() noexcept;
  Minutes get() const noexcept;

private:
  Minutes m_period = DEFAULT;

};

}