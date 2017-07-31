#pragma once
#include "Palette.h"

namespace ClockPlus::Background {

class Background {

public:
  using Id = std::string;

private:
  static constexpr Color DEFAULT_COLOR {0x00, 0x00, 0x00};

public:
  ~Background() = default;
  Background(const Background& b)
      : m_id{b.m_id} {
    m_image = b.m_image;
    m_color = b.m_color;
    m_palette = b.m_palette;
  }
  Background(Background&&) = delete;
  Background& operator=(const Background&) = delete;
  Background& operator=(Background&&) = delete;
  explicit Background(const Id& id);
  const Id& getId() const noexcept;
  const std::string& getImage() const noexcept;
  Color getColor() const noexcept;
  Color getDominant() const noexcept;
  Color getFraction() const noexcept;
  void setImage(const std::string& image);
  void setColor(Color color);
  void addPaletteEntry(const PaletteEntry& entry);

private:
  const Id m_id;
  std::string m_image;
  Color m_color = DEFAULT_COLOR;
  Palette m_palette;

};

}