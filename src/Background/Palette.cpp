#include "Palette.h"

namespace ClockPlus::Background {

Color Palette::getDominant() const noexcept {
  return m_dominant.getColor();
}

Color Palette::getFraction() const noexcept {
  return m_fraction.getColor();
}

void Palette::add(const PaletteEntry& entry) {
  if(m_colors.empty()) {
    m_dominant = entry;
    m_fraction = entry;
  }

  m_colors.push_back(entry);
}

}