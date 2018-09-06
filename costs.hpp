#ifndef COSTS_HPP
#define COSTS_HPP

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
using ActionCostMap = std::map<std::string, Time>;
using ParamMap = std::map<std::string, unsigned int>;

class CostsByEventId {
  CostMap costsById_;

public:
  Time getCostFor(const WCAEventKind k);
  CostsByEventId(const std::string &filename, const std::string &node);
};

class ScramblingCosts : public CostsByEventId  {
public:
  ScramblingCosts(const std::string &filename) : CostsByEventId(filename, "scrambling_costs") {};
};

// FIXME: this should be properly modeled
class SolvingCosts : public CostsByEventId  {
public:
  SolvingCosts(const std::string &filename) : CostsByEventId(filename, "solving_costs") {};
};

class RunnerSystemCosts {
  ActionCostMap costsByName_;
  ParamMap paramsByName_;

public:
  RunnerSystemCosts(const std::string &filename);
#define RUNNER_SYSTEM_PARAM(Name, MapKey) \
  unsigned int get##Name() { return paramsByName_[#MapKey]; }
#define RUNNER_SYSTEM_ACTION(Name, MapKey) \
  Time get##Name##Cost() { return costsByName_[#MapKey]; }
#include "models.def"
};

class JudgeRunsSystemCosts {
  ActionCostMap costsByName_;

public:
  JudgeRunsSystemCosts(const std::string &filename);
#define JUDGE_RUNS_SYSTEM_ACTION(Name, MapKey) \
  Time get##Name##Cost() { return costsByName_[#MapKey]; }
#include "models.def"
};

#endif
