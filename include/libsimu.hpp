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

// Depending on the model used, 'Runners' may be unused!
ErrCodeTy reconfigureStaff(unsigned Judges, unsigned Scramblers, unsigned Runners);

// By default both cutoff and time limit is 600 (a cutoff of 600 is equivalent to
// no cutoff in the context of this program).
ErrCodeTy reconfigureRound(Time Cutoff, Time TimeLimit = 600);

// Experimental stuff
ErrCodeTy reconfigureStats(unsigned ExtraRate, unsigned MiscrambleRate);

// Entry point with a user specificed group of times
TimeResult simuGroup(const std::string &EventId,
  const TimeVector &Times, const std::string &ModelId = DefaultSimulator);

// For a given amount of staff, explore all the (judge, scramblers, runners) to
// find the shortest combination.
OptResult optimizeStaff(const std::string &EventId,
  const TimeVector &Times, unsigned MaxJudges, unsigned TotalStaff,
  const std::string &ModelId = DefaultSimulator);

}



#endif
