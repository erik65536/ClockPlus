#pragma once
#include <string>

namespace ClockPlus::Background {

class Color {

public:
  using Channel = uint8_t;

private:
  static constexpr Channel DEFAULT_R = 0;
  static constexpr Channel DEFAULT_G = 0;
  static constexpr Channel DEFAULT_B = 0;

public:
  constexpr Color() noexcept = default;
  constexpr Color(Channel red, Channel green, Channel blue) noexcept
  : m_r{red}, m_g{green}, m_b{blue} {}
  std::string getHex() const;

public:
  static Color fromString(const std::string& string);

private:
  static Color parseHex6(const std::string& string);
  static Color parseHex3(const std::string& string);
  static char nibbleToHex(uint8_t nibble);
  static uint8_t hexToNibble(char hex);

private:
  Channel m_r = DEFAULT_R;
  Channel m_g = DEFAULT_G;
  Channel m_b = DEFAULT_B;

};

}