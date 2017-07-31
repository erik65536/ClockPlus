#pragma once
#include "Backgrounds.h"
#include <json.hpp>
#include <unordered_map>

namespace ClockPlus::Background {

using Json = nlohmann::json;

class JsonLoader {

private:
  using Id = std::string;
  using BackgroundMap = std::unordered_map<Id, size_t>;
  using Group = std::unordered_set<size_t>;
  using GroupMap = std::unordered_map<Id, Group>;

public:
  static void load(const std::string& json,
      const std::string& image_base,
      Backgrounds& backgrounds);

private:
  static void parse(
      const Json& json,
      const std::string& image_base,
      Backgrounds& backgrounds);
  static void parsePeriod(const Json& json, Backgrounds& backgrounds);
  static void parseBackgrounds(
      const Json& json,
      const std::string& image_base,
      BackgroundMap& map_back,
      Backgrounds& backgrounds);
  static void parseBackground(
      const Json& json,
      const Id& id,
      const std::string& image_base,
      BackgroundMap& map_back,
      Backgrounds& backgrounds);
  static void parseBackgroundImage(
      const Json& json,
      const std::string& image_base,
      Background& background);
  static void parseBackgroundColor(const Json& json, Background& background);
  static void parseBackgroundPalette(const Json& json, Background& background);
  static void parseGroups(
      const Json& json,
      const BackgroundMap& map_back,
      GroupMap& map_group);
  static void parseGroup(
      const Json& json,
      const Id& id,
      const BackgroundMap& map_back,
      GroupMap& map_group);
  static void parseGroupBackground(
      const Id& id_group,
      const Id& id_back,
      const BackgroundMap& map_back,
      Group& group);
  static void parseFilters(
      const Json& json,
      const BackgroundMap& map_back,
      const GroupMap& map_group,
      Backgrounds& backgrounds);
  static void parseFilter(
      const Json& json,
      const BackgroundMap& map_back,
      const GroupMap& map_group,
      Backgrounds& backgrounds);
  static Filter::Priority parseFilterPriority(const Json& json);
  static void parseFilterMessage(const Json& json, Filter& filter);
  static void parseFilterBackgrounds(
      const Json& json,
      const BackgroundMap& map_back,
      const GroupMap& map_group,
      Filter& filter);
  static void parseFilterDates(const Json& json, Filter& filter);
  static void parseFilterDate(const Json& json, Date& date);
  template<typename Func>
  static void parseDateComponent(
      const Json& json,
      const std::string& key,
      Func func);

};

}