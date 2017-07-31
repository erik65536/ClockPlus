#pragma once
#include "Color.h"

namespace ClockPlus::Background {

class PaletteEntry {

private:
  static constexpr Color DEFAULT_COLOR = Color(0xff, 0xff, 0xff);
  static constexpr double DEFAULT_SCORE = 0.0;
  static constexpr double DEFAULT_FRACTION = 0.0;

public:
  constexpr PaletteEntry() noexcept  = default;
  constexpr PaletteEntry(
      const Color& color,
      double score,
      double fraction)noexcept
  : m_color(color), m_score{score}, m_fraction{fraction} {}
  Color getColor() const noexcept;
  void setColor(Color color) noexcept;
  double getScore() const noexcept;
  void setScore(double score) noexcept;
  double getFraction() const noexcept;
  void setFraction(double fraction) noexcept;

private:
  Color m_color = DEFAULT_COLOR;
  double m_score = DEFAULT_SCORE;
  double m_fraction = DEFAULT_FRACTION;

};

}