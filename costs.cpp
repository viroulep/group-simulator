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
#define EVENT(Id, Name, MaxAttempts, Rank) \
    case WCAEvent##Id:        \
      return #Name;
#include "events.def"
  }
  // unreachable
  assert(false);
  return "";
}

unsigned int getMaxAttemptsFor(WCAEventKind t)
{
  switch (t) {
    case WCAEventUnknown:
      return 5;
#define EVENT(Id, Name, MaxAttempts, Rank) \
    case WCAEvent##Id:        \
      return MaxAttempts;
#include "events.def"
  }
  // unreachable
  assert(false);
  return 5;
}

WCAEventKind getWCAEventKindFromId(const std::string &id)
{
#define EVENT(Id, Name, MaxAttempts, Rank) \
  if (id == #Id)              \
    return WCAEvent##Id;
#include "events.def"
  return WCAEventUnknown;
}

Time CostsByEventId::getCostFor(const WCAEventKind k)
{
  return costsById_[k];
}

// FIXME: dirty code duplication below

CostsByEventId::CostsByEventId(const string &filename, const string &node)
{
  // TODO proper error handling
  YAML::Node config = YAML::LoadFile(filename);
  auto costs = config[node].as<map<string, Time>>();
#define EVENT(Id, Name, MaxAttempts, Rank)  \
  if (costs.count(#Id) != 1) { \
    cerr << "Couldn't find costs for " << getWCAEventName(WCAEvent##Id) << "\n"; \
  } else { \
    costsById_[WCAEvent##Id] = costs[#Id]; \
  }
#include "events.def"

  /*for (auto &pair : costsById_) {
    cout << "Cost for " << getWCAEventName(pair.first) << ": " << to_string(pair.second) << " seconds.\n";
  }*/
}


RunnerSystemCosts::RunnerSystemCosts(const string &filename)
{
  // TODO proper error handling
  YAML::Node config = YAML::LoadFile(filename);
  auto costs = config["runner_system"]["actions"].as<map<string, Time>>();
  auto params = config["runner_system"]["params"].as<map<string, unsigned int>>();
#define RUNNER_SYSTEM_ACTION(Name, MapKey)  \
  if (costs.count(#MapKey) != 1) { \
    cerr << "Couldn't find action cost for " << #Name << "\n"; \
  } else { \
    costsByName_[#MapKey] = costs[#MapKey]; \
  }
#define RUNNER_SYSTEM_PARAM(Name, MapKey)  \
  if (params.count(#MapKey) != 1) { \
    cerr << "Couldn't find param " << #Name << "\n"; \
  } else { \
    paramsByName_[#MapKey] = params[#MapKey]; \
  }
#include "models.def"

  /*for (auto &pair : costsByName_) {
    cout << "Action cost for " << pair.first << ": " << to_string(pair.second) << " seconds.\n";
  }
  for (auto &pair : paramsByName_) {
    cout << "Param " << pair.first << ": " << to_string(pair.second) << ".\n";
  }*/
}

JudgeRunsSystemCosts::JudgeRunsSystemCosts(const string &filename)
{
  // TODO proper error handling
  YAML::Node config = YAML::LoadFile(filename);
  auto costs = config["judge_runs_system"]["actions"].as<map<string, Time>>();
#define JUDGE_RUNS_SYSTEM_ACTION(Name, MapKey)  \
  if (costs.count(#MapKey) != 1) { \
    cerr << "Couldn't find action cost for " << #Name << "\n"; \
  } else { \
    costsByName_[#MapKey] = costs[#MapKey]; \
  }
#include "models.def"

  /*for (auto &pair : costsByName_) {
    cout << "Action cost for " << pair.first << ": " << to_string(pair.second) << " seconds.\n";
  }*/
}
