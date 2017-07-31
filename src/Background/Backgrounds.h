#pragma once
#include "Period.h"
#include "Background.h"
#include "Filter.h"
#include <unordered_set>
#include <map>

namespace ClockPlus::Background {

class Backgrounds {

public:
  using Id = std::string;
  using IdVec = std::vector<Id>;
  using IdSet = std::unordered_set<Id>;
  using BackgroundVec = std::vector<Background>;
  using FilterMap = std::multimap<Filter::Priority, Filter, std::greater<>>;
  using FilterMapIt = FilterMap::const_iterator;

public:
  Period::Minutes getPeriod() const noexcept;
  void setPeriod(Period::Minutes minutes) noexcept;
  std::tuple<Background&, size_t> addBackground(const Id& id);
  const BackgroundVec& getBackgrounds() const noexcept;
  Filter& addFilter(Filter::Priority priority);
  void clear();
  bool match(const DateTimeProvider& date, std::string& message, IdVec& ids) const;

private:
  std::tuple<FilterMapIt, bool> matchPriority(
      const DateTimeProvider& date,
      FilterMapIt iterator,
      FilterMapIt end,
      std::string& message,
      IdSet& matches) const;
  bool matchFilter(
      const DateTimeProvider& date,
      const Filter& filter,
      std::string& message,
      IdSet& matches) const;
  static bool equalSet(const IdSet& set1, const IdVec& set2);

private:
  Period        m_period;
  BackgroundVec m_backgrounds;
  FilterMap     m_filters;

};

}