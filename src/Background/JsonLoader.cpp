#include "JsonLoader.h"
#include <gtkmm.h>

namespace ClockPlus::Background {

using namespace std::string_literals;

void JsonLoader::load(
    const std::string& json_string,
    const std::string& image_base,
    Backgrounds& backgrounds) {
  backgrounds.clear();
  auto json = Json::parse(json_string);
  parse(json, image_base, backgrounds);
}

void JsonLoader::parse(
    const Json& json,
    const std::string& image_base,
    Backgrounds& backgrounds) {
  parsePeriod(json, backgrounds);
  BackgroundMap map_back;
  parseBackgrounds(json, image_base, map_back, backgrounds);
  GroupMap map_group;
  parseGroups(json, map_back, map_group);
  parseFilters(json, map_back, map_group, backgrounds);
}

void JsonLoader::parsePeriod(const Json& json, Backgrounds& backgrounds) {
  auto json_period = json.find("period");

  if(json_period == json.end()) {
    return;
  }

  backgrounds.setPeriod(*json_period);
}

void JsonLoader::parseBackgrounds(
    const Json& json,
    const std::string& base,
    BackgroundMap& map_back,
    Backgrounds& backgrounds) {
  auto json_backgrounds = json.find("backgrounds");

  if(json_backgrounds == json.end()) {
    return;
  }

  for(auto background = json_backgrounds->cbegin();
      background != json_backgrounds->cend();
      ++background) {
    parseBackground(
        background.value(),
        background.key(),
        base,
        map_back,
        backgrounds);
  }
}

void JsonLoader::parseBackground(
    const Json& json,
    const Id& id,
    const std::string& image_base,
    BackgroundMap& map_back,
    Backgrounds& backgrounds) {

  auto tuple_background = backgrounds.addBackground(id);
  Background& background = std::get<0>(tuple_background);
  size_t index = std::get<1>(tuple_background);

  bool inserted;
  std::tie(std::ignore, inserted) = map_back.insert(std::make_pair(id, index));
  if(!inserted) {
    throw std::invalid_argument("Duplicate background ids \""s + id + "\""s);
  }

  parseBackgroundImage(json, image_base, background);
  parseBackgroundColor(json, background);
  parseBackgroundPalette(json, background);
}

void JsonLoader::parseBackgroundImage(
    const Json& json,
    const std::string& image_base,
    Background& background) {
  auto json_image = json.find("image");

  if(json_image == json.end()) {
    return;
  }

  std::string image = *json_image;
  if(!Glib::path_is_absolute(image)) {
    image = Glib::build_filename(image_base, image);
  }

  background.setImage(image);
}

void JsonLoader::parseBackgroundColor(
    const Json& json,
    Background& background) {
  auto json_color = json.find("color");

  if(json_color == json.end()) {
    return;
  }

  background.setColor(Color::fromString(*json_color));
}

void JsonLoader::parseBackgroundPalette(
    const Json& json,
    Background& background) {
  auto json_palette = json.find("palette");

  if(json_palette == json.end()) {
    return;
  }

  for(const auto& json_entry : *json_palette) {
    PaletteEntry entry;

    auto json_color = json_entry.find("color");
    if(json_color != json_entry.end()) {
      entry.setColor(Color::fromString(*json_color));
    }

    auto json_score = json_entry.find("score");
    if(json_score != json_entry.end()) {
      entry.setScore(*json_score);
    }

    auto json_fraction = json_entry.find("fraction");
    if(json_fraction != json_entry.end()) {
      entry.setFraction(*json_fraction);
    }

    background.addPaletteEntry(entry);
  }
}

void JsonLoader::parseGroups(
    const Json& json,
    const BackgroundMap& map_back,
    GroupMap& map_group) {
  auto json_groups = json.find("groups");

  if(json_groups == json.end()) {
    return;
  }

  for(auto group = json_groups->cbegin();
      group != json_groups->cend();
      ++group) {
    parseGroup(group.value(), group.key(), map_back, map_group);
  }
}

void JsonLoader::parseGroup(
    const Json& json,
    const Id& id,
    const BackgroundMap& map_back,
    GroupMap& map_group) {

  if(map_back.count(id) != 0) {
    throw std::invalid_argument("Duplicate ids \""s +id + "\""s);
  }


  GroupMap::iterator it_group;
  bool inserted;
  std::tie(it_group, inserted) = map_group.insert(std::make_pair(id, Group()));
  if(!inserted) {
    throw std::invalid_argument("Duplicate group ids \""s + id + "\""s);
  }
  auto& group = it_group->second;

  for(const auto& json_background : json) {
    std::string id_back = json_background;
    parseGroupBackground(id, id_back, map_back, group);
  }
}

void JsonLoader::parseGroupBackground(
    const Id& id_group,
    const Id& id_back,
    const BackgroundMap& map_back,
    Group& group) {

  auto it_back = map_back.find(id_back);
  if(it_back == map_back.end()) {
    throw std::invalid_argument(
        "Group \""s + id_group +"\" references undefined background \""s +
        id_back + "\""s);
  }

  bool inserted;
  std::tie(std::ignore, inserted) = group.emplace(it_back->second);
  if(!inserted) {
    throw std::invalid_argument(
        "Duplicate background \""s + id_back +
        "\" in group \""s + id_group + "\""s);
  }
}

void JsonLoader::parseFilters(
    const Json& json,
    const BackgroundMap& map_back,
    const GroupMap& map_group,
    Backgrounds& backgrounds) {
  auto json_filters = json.find("filters");

  if(json_filters == json.end()) {
    return;
  }

  for(const auto& json_filter : *json_filters) {
    parseFilter(json_filter, map_back, map_group, backgrounds);
  }
}

void JsonLoader::parseFilter(
    const Json& json,
    const BackgroundMap& map_back,
    const GroupMap& map_group,
    Backgrounds& backgrounds) {
  Filter::Priority priority = parseFilterPriority(json);
  Filter& filter = backgrounds.addFilter(priority);
  parseFilterMessage(json, filter);
  parseFilterBackgrounds(json, map_back, map_group, filter);
  parseFilterDates(json, filter);
}

Filter::Priority JsonLoader::parseFilterPriority(const Json& json) {
  auto json_priority = json.find("priority");

  if(json_priority == json.end()) {
    return Filter::DEFAULT_PRIORITY;
  }

  return *json_priority;
}

void JsonLoader::parseFilterMessage(const Json& json, Filter& filter) {
  auto json_message = json.find("message");

  if(json_message == json.end()) {
    return;
  }

  filter.setMessage(*json_message);
}

void JsonLoader::parseFilterBackgrounds(
    const Json& json,
    const BackgroundMap& map_back,
    const GroupMap& map_group,
    Filter& filter) {
  auto json_backgrounds = json.find("backgrounds");

  if(json_backgrounds == json.end()) {
    return;
  }

  for(const auto& json_background : *json_backgrounds) {
    std::string id = json_background;

    auto it_back = map_back.find(id);
    if(it_back != map_back.end()) {
      filter.addBackground(it_back->second);
      continue;
    }

    auto it_group = map_group.find(id);
    if(it_group != map_group.end()) {
      for(auto background : it_group->second) {
        filter.addBackground(background);
      }
      continue;
    }

    throw std::invalid_argument("Filter references undefined "s + id);
  }
}

void JsonLoader::parseFilterDates(const Json& json, Filter& filter) {
  auto json_dates = json.find("dates");

  if(json_dates == json.end()) {
    return;
  }

  for(const auto& json_date : *json_dates) {
    Date& date = filter.addDate();
    parseFilterDate(json_date, date);
  }
}

void JsonLoader::parseFilterDate(const Json& json, Date& date) {
  parseDateComponent(json, "tag", [&date] (const std::string& tag) {
    date.addTag(tag);
  });

  parseDateComponent(json, "year", [&date] (int year) {
    date.addYear(year);
  });

  parseDateComponent(json, "month", [&date] (int month) {
    date.addMonth(month);
  });

  parseDateComponent(json, "dayOfMonth", [&date] (int day_of_month) {
    date.addDayOfMonth(day_of_month);
  });

  parseDateComponent(json, "dayOfWeek", [&date] (int day_of_week) {
    date.addDayOfWeek(day_of_week);
  });
}

template<typename Func>
void JsonLoader::parseDateComponent(
    const Json& json,
    const std::string& key,
    Func func) {
  auto json_key = json.find(key);

  if(json_key == json.end()) {
    return;
  }

  if(!json_key->is_array()) {
    func(*json_key);
    return;
  }

  for(const auto& json_item : *json_key) {
    func(json_item);
  }
}

}