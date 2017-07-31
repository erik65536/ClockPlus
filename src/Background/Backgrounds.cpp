#include "Backgrounds.h"

namespace ClockPlus::Background {

using namespace std::string_literals;

Period::Minutes Backgrounds::getPeriod() const noexcept {
  return m_period.get();
}

void Backgrounds::setPeriod(Period::Minutes minutes) noexcept {
  m_period.set(minutes);
}

const Backgrounds::BackgroundVec& Backgrounds::getBackgrounds() const noexcept {
  return m_backgrounds;
}

std::tuple<Background&, size_t> Backgrounds::addBackground(const Id& id) {
  m_backgrounds.emplace_back(id);
  return std::make_tuple(
      std::ref(m_backgrounds.back()),
      m_backgrounds.size()-1);
}

Filter& Backgrounds::addFilter(Filter::Priority priority) {
  return m_filters.emplace(
      std::make_pair<Filter::Priority, Filter::Priority>(
          std::move(priority), std::move(priority)))->second;
}

void Backgrounds::clear() {
  m_filters.clear();
  m_backgrounds.clear();
  m_period.setDefault();
}

bool Backgrounds::match(
    const DateTimeProvider& date,
    std::string& message,
    IdVec& backgrounds) const {
  message.clear();

  IdSet matches;

  for(auto iterator = m_filters.cbegin();
      iterator != m_filters.cend();) {
    bool match;
    std::tie(iterator, match) = matchPriority(
        date,
        iterator,
        m_filters.cend(),
        message,
        matches);

    if(match) {
      break;
    }
  }

  if(equalSet(matches, backgrounds)) {
    return false;
  }

  backgrounds.assign(matches.cbegin(), matches.cend());

  return true;
}

std::tuple<Backgrounds::FilterMapIt, bool> Backgrounds::matchPriority(
    const DateTimeProvider& date,
    FilterMapIt iterator,
    FilterMapIt end,
    std::string& message,
    IdSet& matches) const {

  if(iterator == end) {
    return std::make_tuple(iterator, false);
  }

  auto priority = iterator->first;
  bool match = false;

  while(iterator != end) {
    match |= matchFilter(date, iterator->second, message, matches);
    ++iterator;
    if(priority != iterator->first) {
      break;
    }
  }

  return std::make_tuple(iterator, match);
};

bool Backgrounds::matchFilter(
    const DateTimeProvider& date,
    const Filter& filter,
    std::string& message,
    IdSet& matches) const {

  if(!filter.isToday(date)) {
    return false;
  }

  if(message.empty()) {
    message.assign(filter.getMessage());
  }

  for(auto index : filter.getBackgrounds()) {
    matches.emplace(m_backgrounds[index].getId());
  }

  return true;
}

bool Backgrounds::equalSet(const IdSet& set1, const IdVec& set2) {
  if(set1.size() != set2.size()) {
    return false;
  }

  for(const auto& item : set2) {
    if(set1.count(item) == 0) {
      return false;
    }
  }

  return true;
}

}