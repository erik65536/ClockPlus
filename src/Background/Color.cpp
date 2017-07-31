#include "Color.h"
#include <array>

namespace ClockPlus::Background {

using namespace std::string_literals;

std::string Color::getHex() const {

  std::array<char, 7> buffer {{
      '#',
      nibbleToHex(m_r >> 4),
      nibbleToHex(static_cast<uint8_t>(m_r & 0x0f)),
      nibbleToHex(m_g >> 4),
      nibbleToHex(static_cast<uint8_t>(m_g & 0x0f)),
      nibbleToHex(m_b >> 4),
      nibbleToHex(static_cast<uint8_t>(m_b & 0x0f))
  }};

  return std::string(buffer.data(), buffer.size());
}

Color Color::fromString(const std::string& string) {
  try {
    if(string.length() == 7) {
      return parseHex6(string);
    }

    if(string.length() == 4) {
      return parseHex3(string);
    }
  } catch(const std::invalid_argument& error) {
    (void)error;
  }

  throw std::invalid_argument("Invalid color "s + string);
}

Color Color::parseHex6(const std::string& string) {
  uint8_t r = static_cast<uint8_t>(hexToNibble(
      string[1]) << 4) | hexToNibble(string[2]);
  uint8_t g = static_cast<uint8_t>(hexToNibble(
      string[3]) << 4) | hexToNibble(string[4]);
  uint8_t b = static_cast<uint8_t>(hexToNibble(
      string[5]) << 4) | hexToNibble(string[6]);

  return Color(r, g, b);
}

Color Color::parseHex3(const std::string& string) {
  uint8_t r = hexToNibble(string[1]);
  r = static_cast<uint8_t>(r << 4) | r;
  uint8_t g = hexToNibble(string[2]);
  g = static_cast<uint8_t>(g << 4) | g;
  uint8_t b = hexToNibble(string[3]);
  b = static_cast<uint8_t>(b << 4) | b;

  return Color(r, g, b);
}

char Color::nibbleToHex(uint8_t nibble) {
  if(nibble <= 9) {
    return '0' + static_cast<char>(nibble);
  }
  return 'a' + static_cast<char>((nibble & 0xf) - 10);
}

uint8_t Color::hexToNibble(char hex) {
  if(hex < '0') {
    throw std::invalid_argument(nullptr);
  }
  if(hex <= '9') {
    return static_cast<uint8_t>(hex - '0');
  }
  if(hex < 'a') {
    throw std::invalid_argument(nullptr);
  }
  if(hex <= 'f') {
    return static_cast<uint8_t>(0xa + hex - 'a');
  }
  if(hex < 'A') {
    throw std::invalid_argument(nullptr);
  }
  if(hex <= 'F') {
    return static_cast<uint8_t>(0xa + hex - 'A');
  }
  throw std::invalid_argument(nullptr);
}

}