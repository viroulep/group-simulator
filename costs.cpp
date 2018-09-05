#include "costs.hpp"
#include <cassert>
#include <map>
#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace std;

string getWCAEventName(WCAEventKind t)
{
  switch (t) {
    case WCAEventUnknown:
      return "Unknown";
#define EVENT(Id, Name, Rank) \
    case WCAEvent##Id:        \
      return #Name;
#include "events.def"
  }
  // unreachable
  assert(false);
  return "";
}

WCAEventKind getWCAEventKindFromId(const std::string &id)
{
#define EVENT(Id, Name, Rank) \
  if (id == #Id)              \
    return WCAEvent##Id;
#include "events.def"
  return WCAEventUnknown;
}

Time ScramblingCosts::getScramblingTimeFor(const WCAEventKind k)
{
  return costsById_[k];
}

ScramblingCosts::ScramblingCosts(const std::string &filename)
{
  // TODO proper error handling
  YAML::Node config = YAML::LoadFile(filename);
  auto costs = config["scrambling_costs"].as<map<string, Time>>();
#define EVENT(Id, Name, Rank)  \
  if (costs.count(#Id) != 1) { \
    cerr << "Couldn't find scrambling costs for " << getWCAEventName(WCAEvent##Id) << "\n"; \
  } else { \
    costsById_[WCAEvent##Id] = costs[#Id]; \
  }
#include "events.def"

  for (auto &pair : costsById_) {
    cout << "Scrambling cost for " << getWCAEventName(pair.first) << ": " << to_string(pair.second) << " seconds.\n";
  }
}
