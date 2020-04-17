#ifndef LIBSIMU_HPP
#define LIBSIMU_HPP

#include <string>
#include <vector>
#include <map>

namespace libsimu {
namespace errors {
  enum ErrorKind {
    SUCCESS = 0,
    GENERIC,
    SIMULATION_FAILURE,
    INVALID_EVENT,
    INVALID_CONFIG,
    INVALID_FILE,
  };

  std::string errorMessage(ErrorKind K);
}

// Because it is used for a Wasm target, this header must use only basic types.
// (and std map/vector)

using Time = unsigned long;
using ErrCodeTy = errors::ErrorKind;
using PropertiesMap = std::map<std::string, unsigned long>;
using TimeVector = std::vector<Time>;

extern const std::string DefaultSimulator;

struct OptResult {
  ErrCodeTy Err;
  Time BestResult;
  unsigned Judges;
  unsigned Runners;
  unsigned Scramblers;
};

struct TimeResult {
  ErrCodeTy Err;
  Time Value;
};

ErrCodeTy loadConfig(PropertiesMap const &Setup, PropertiesMap const &Model,
    PropertiesMap const &Scrambling);

PropertiesMap getSetupProps();
PropertiesMap getModelProps();
PropertiesMap getScramblingProps();

// Entry point with a user specificed group of times
TimeResult simuGroup(std::string const &EventId,
  TimeVector const &Times, PropertiesMap const &SetupOverride,
  std::string const &ModelId);

// For a given amount of staff, explore all the (judge, scramblers, runners) to
// find the shortest combination.
OptResult optimizeStaff(const std::string &EventId,
  const TimeVector &Times, unsigned MaxJudges, unsigned TotalStaff,
  PropertiesMap const &SetupOverride,
  const std::string &ModelId);

}



#endif
