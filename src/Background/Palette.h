#pragma once
#include "PaletteEntry.h"
#include <vector>

namespace ClockPlus::Background {

class Palette {

private:
  using ColorVec = std::vector<PaletteEntry>;
  static constexpr Color DEFAULT_DOMINANT = Color(0xff, 0xff, 0xff);
  static constexpr Color DEFAULT_FRACTION = Color(0x00, 0x00, 0x00);
  static constexpr PaletteEntry ENTRY_DOMINANT = PaletteEntry(
      DEFAULT_DOMINANT,
      1.0,
      1.0);
  static constexpr PaletteEntry ENTRY_FRACTION = PaletteEntry(
      DEFAULT_FRACTION,
      1.0,
      1.0);

public:
  Color getDominant() const noexcept;
  Color getFraction() const noexcept;
  void add(const PaletteEntry& entry);

private:
  ColorVec m_colors;
  PaletteEntry m_dominant = ENTRY_DOMINANT;
  PaletteEntry m_fraction = ENTRY_FRACTION;

};

}