#ifndef LIBSIMU_HPP
#define LIBSIMU_HPP

#include <string>
#include <vector>
#include <map>

namespace libsimu {

using Time = uint64_t;
using ErrCodeTy = int;
using PropertiesMap = std::map<std::string, std::uint64_t>;
// TODO: define error codes/string

extern const std::string DefaultSimulator;

struct OptResult {
  ErrCodeTy Err;
  Time BestResult;
  uint8_t Judges;
  uint8_t Runners;
  uint8_t Scramblers;
};

struct TimeResult {
  ErrCodeTy Err;
  Time Value;
};

ErrCodeTy LoadConfig(PropertiesMap const &Setup, PropertiesMap const &Model,
    PropertiesMap const &Scrambling);

ErrCodeTy TestStuff(int a);
void EmitConfig();

// Depending on the model used, 'Runners' may be unused!
ErrCodeTy ReconfigureStaff(uint8_t Judges, uint8_t Scramblers, uint8_t Runners);

// By default both cutoff and time limit is 600 (a cutoff of 600 is equivalent to
// no cutoff in the context of this program).
ErrCodeTy ReconfigureRound(Time Cutoff, Time TimeLimit = 600);

// Experimental stuff
ErrCodeTy ReconfigureStats(uint8_t ExtraRate, uint8_t MiscrambleRate);

// Entry point with a user specificed group of times
ErrCodeTy SimuGroup(Time *Result, const std::string &EventId,
  const std::vector<Time> &Times, const std::string &ModelId = DefaultSimulator);

// For a given amount of staff, explore all the (judge, scramblers, runners) to
// find the shortest combination.
OptResult OptimizeStaff(const std::string &EventId,
  const std::vector<Time> &Times, uint8_t MaxJudges, uint8_t TotalStaff,
  const std::string &ModelId = DefaultSimulator);

}



#endif
