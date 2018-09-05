#include "types.hpp"
#include <string>
#include <map>

enum WCAEventKind {
#define EVENT(Id, Name, Rank) \
  WCAEvent##Id,
#include "events.def"
  WCAEventUnknown,
};

std::string getEventTypeName(WCAEventKind t);

using CostMap = std::map<WCAEventKind, Time>;

class ScramblingCosts {
  CostMap costsById_;

public:
  Time getScramblingTimeFor(const WCAEventKind k);
  ScramblingCosts(const std::string &filename);
};
