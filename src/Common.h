#pragma once
#include <string>

namespace ClockPlus {

using zstring = char*;
using czstring = const char*;

constexpr czstring APPLICATION_NAME = "ClockPlus";

const std::string& getConfigDir();

}