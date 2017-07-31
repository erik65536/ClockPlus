#include "PaletteEntry.h"

namespace ClockPlus::Background {

Color PaletteEntry::getColor() const noexcept {
  return m_color;
}

double PaletteEntry::getScore() const noexcept {
  return m_score;
}

double PaletteEntry::getFraction() const noexcept {
  return m_fraction;
}

void PaletteEntry::setColor(Color color) noexcept {
  m_color = color;
}

void PaletteEntry::setScore(double score) noexcept {
  m_score = score;
}

void PaletteEntry::setFraction(double fraction) noexcept {
  m_fraction = fraction;
}

}