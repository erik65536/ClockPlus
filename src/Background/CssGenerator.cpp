#include "CssGenerator.h"

namespace ClockPlus::Background {

using namespace std::string_literals;

std::string classBackground(const std::string& id) {
  static const std::string base = "back-"s;
  return base + id;
}

std::string classLabel(const std::string& id) {
  static const std::string base = "label-";
  return base + id;
}

std::string CssGenerator::generate(const Backgrounds& backgrounds) {
  std::string css;

  for(const auto& background : backgrounds.getBackgrounds()) {
    css
      .append("."s).append(classBackground(background.getId()))
      .append("{background-color:"s)
      .append(background.getFraction().getHex())
      .append(";background-image:url(\'"s)
      .append(background.getImage())
      .append("\');}")

      .append("."s).append(classLabel(background.getId()))
      .append("{color:"s)
      .append(background.getDominant().getHex())
      .append(";}"s);
  }

  return css;
}

}