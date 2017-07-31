#include "Period.h"
#include <string>
#include <stdexcept>

namespace ClockPlus::Background {

using namespace std::string_literals;

void Period::set(Period::Minutes period) {
  if(period < MIN) {
    static_assert(MIN == 1, "Update the error string for the new constant.");
    throw std::invalid_argument("The period must be at least one 1 minute"s);
  }

  m_period = period;
}

void Period::setDefault() noexcept {
  m_period = DEFAULT;
}

Period::Minutes Period::get() const noexcept {
  return m_period;
}

}