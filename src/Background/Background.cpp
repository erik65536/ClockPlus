#include "Background.h"

namespace ClockPlus::Background {

Background::Background(const Id& id)
: m_id{id} {}

const Background::Id& Background::getId() const noexcept {
  return m_id;
}

const std::string& Background::getImage() const noexcept {
  return m_image;
}

Color Background::getColor() const noexcept {
  if(m_image.empty()) {
    return m_color;
  }
  return getFraction();
}

Color Background::getDominant() const noexcept {
  return m_palette.getDominant();
}

Color Background::getFraction() const noexcept {
  return m_palette.getFraction();
}

void Background::setImage(const std::string& image) {
  m_image = image;
}

void Background::setColor(Color color) {
  m_color = color;
}

void Background::addPaletteEntry(const PaletteEntry& entry) {
  m_palette.add(entry);
}

}
