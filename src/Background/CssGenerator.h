#pragma once
#include "Backgrounds.h"

namespace ClockPlus::Background {

std::string classBackground(const std::string& id);
std::string classLabel(const std::string& id);

class CssGenerator {

public:
  static std::string generate(const Backgrounds& backgrounds);

};

}