#ifndef LIBSIMU_HPP
#define LIBSIMU_HPP

#include <string>
#include <vector>
#include <system_error>

namespace libsimu {

using Time = uint64_t;
extern const std::string DefaultSimulator;

enum class JudgesParam : std::uint8_t {};
enum class RunnersParam : std::uint8_t {};
enum class ScramblersParam : std::uint8_t {};

template<typename T>
std::uint8_t as_integer(T x) { return static_cast<std::uint8_t>(x); }
template<typename T>
T as(std::uint8_t x) { return T{x}; }


struct OptResult {
  Time BestResult;
  uint8_t Judges;
  uint8_t Runners;
  uint8_t Scramblers;
};

std::error_code LoadConfig(const std::string &Filename);

int TestStuff(int a);
void EmitConfig(std::ostream &out);

// Depending on the model used, 'Runners' may be unused!
std::error_code ReconfigureStaff(JudgesParam J, ScramblersParam S,
  RunnersParam R);

// By default both cutoff and time limit is 600 (a cutoff of 600 is equivalent to
// no cutoff in the context of this program).
std::error_code ReconfigureRound(Time Cutoff, Time TimeLimit = 600);

// Experimental stuff
std::error_code ReconfigureStats(uint8_t ExtraRate, uint8_t MiscrambleRate);

// Entry point with a user specificed group of times
std::error_code SimuGroup(Time *Result, const std::string &EventId,
  const std::vector<Time> &Times, const std::string &ModelId = DefaultSimulator);

// For a given amount of staff, explore all the (judge, scramblers, runners) to
// find the shortest combination.
std::error_code OptimizeStaff(OptResult *Result, const std::string &EventId,
  const std::vector<Time> &Times, uint8_t MaxJudges, uint8_t TotalStaff,
  const std::string &ModelId = DefaultSimulator);

}



#endif
