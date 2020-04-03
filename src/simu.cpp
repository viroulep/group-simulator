#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/YAMLTraits.h>
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <cassert>
#include <iterator>

#include "libsimu.hpp"

using namespace llvm;
using namespace std;
using namespace libsimu;

// Positional and mandatory
static cl::opt<std::string> EventId(cl::Positional, cl::desc("<wca_event_id>"), cl::Required);

// Options
static cl::opt<std::string> ConfigPath("config", cl::desc("Specify path to (maybe partial) config"), cl::value_desc("filename"));
static cl::opt<std::string> Psychsheet("list", cl::desc("Specify path to the list of times"), cl::value_desc("filename"));

static cl::opt<std::string> ModelId("m", cl::desc("Set the simulator to use (Runners, JudgesRun)"), cl::value_desc("model_id"), cl::init("Runners"));

static cl::opt<unsigned> Judges("j", cl::desc("Overrides the number of judges"), cl::value_desc("judges"));
static cl::opt<unsigned> Scramblers("s", cl::desc("Overrides the number of scramblers"), cl::value_desc("scramblers"));
static cl::opt<unsigned> Runners("r", cl::desc("Overrides the number of runners"), cl::value_desc("runners"));
static cl::opt<unsigned> CubesPerRunner("r-cubes", cl::desc("Set the number of cubes per runner"), cl::value_desc("cubes"));

static cl::opt<unsigned> TimeLimit("tl", cl::desc("Set the time limit"), cl::value_desc("seconds"), cl::init(600));
static cl::opt<unsigned> Cutoff("cut", cl::desc("Set the cutoff"), cl::value_desc("seconds"), cl::init(600));

static cl::opt<unsigned> GroupSize("size", cl::desc("Set the group size"), cl::value_desc("number"), cl::init(20));
static cl::opt<unsigned> Avg("avg-time", cl::desc("Set the group average time"), cl::value_desc("seconds"), cl::init(10));

// Experimental or not implemented
static cl::opt<unsigned> ExtraRate("extra", cl::desc("Set the extra rate"), cl::value_desc("percentage"), cl::init(0), cl::Hidden);
static cl::opt<unsigned> MiscrambleRate("miscramble", cl::desc("Set the miscramble rate"), cl::value_desc("percentage"), cl::init(0), cl::Hidden);


int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);

  if (ConfigPath.length() > 0) {
    if (auto err = LoadConfig(ConfigPath)) {
      errs() << "Error loading config\n";
      return err.value();
    }
  }



  // Default values are 0, and since they are invalid values they are considered
  // as not overriding the config.
  ReconfigureStaff(Judges, Scramblers, Runners, CubesPerRunner);
  ReconfigureRound(Cutoff, TimeLimit);
  ReconfigureStats(ExtraRate, MiscrambleRate);

  EmitConfig(std::cout);

  std::vector<Time> Times(GroupSize, Avg);

  Time Result;

  SimuGroup(&Result, EventId, Times, ModelId);

  chrono::seconds roundDuration(Result);
  chrono::minutes durationInMinute = chrono::duration_cast<chrono::minutes>(roundDuration);
  chrono::seconds remaining = roundDuration - durationInMinute;
  cout << "Group took " << durationInMinute.count() << " minutes and " << remaining.count() << " seconds.\n";

  return 0;
}
