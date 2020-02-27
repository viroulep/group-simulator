#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/YAMLTraits.h>
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <cassert>
#include <iterator>

#include "libsimu.hpp"

using namespace llvm;
using namespace libsimu;

// Positional and mandatory
static cl::opt<std::string> EventId(cl::Positional, cl::desc("<wca_event_id>"), cl::Required);

// Options
static cl::opt<std::string> CostsModelPath("mcosts", cl::desc("Specify path to model costs"), cl::value_desc("filename"));
static cl::opt<std::string> ScramblingCostsPath("scosts", cl::desc("Specify path to scrambling costs"), cl::value_desc("filename"));
static cl::opt<std::string> ConfigPath("config", cl::desc("Specify path to config"), cl::value_desc("filename"));
static cl::opt<std::string> Psychsheet("list", cl::desc("Specify path to the list of times"), cl::value_desc("filename"));

static cl::opt<std::string> ModelId("m", cl::desc("Set the simulator to use (Runners, JudgesRun)"), cl::value_desc("model_id"), cl::init("Runners"));

static cl::opt<uint8_t> Judges("j", cl::desc("Overrides the number of judges"), cl::value_desc("judges"), cl::init(10));
static cl::opt<uint8_t> Scramblers("s", cl::desc("Overrides the number of scramblers"), cl::value_desc("scramblers"), cl::init(3));
static cl::opt<uint8_t> Runners("r", cl::desc("Overrides the number of runners"), cl::value_desc("runners"), cl::init(2));
static cl::opt<uint8_t> CubesPerRunner("r-cubes", cl::desc("Set the number of cubes per runner"), cl::value_desc("cubes"), cl::init(3));

static cl::opt<Time> TimeLimit("tl", cl::desc("Set the time limit"), cl::value_desc("seconds"), cl::init(600));
static cl::opt<Time> Cutoff("cut", cl::desc("Set the cutoff"), cl::value_desc("seconds"), cl::init(600));

static cl::opt<unsigned int> GroupSize("size", cl::desc("Set the group size"), cl::value_desc("number"), cl::init(20));
static cl::opt<Time> Avg("avg-time", cl::desc("Set the group average time"), cl::value_desc("seconds"), cl::init(10));

// Experimental or not implemented
static cl::opt<uint8_t> ExtraRate("extra", cl::desc("Set the extra rate"), cl::value_desc("percentage"), cl::init(0), cl::Hidden);
static cl::opt<uint8_t> MiscrambleRate("miscramble", cl::desc("Set the miscramble rate"), cl::value_desc("percentage"), cl::init(0), cl::Hidden);


int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);

  if (CostsModelPath.length() > 0) {
    if (auto err = LoadModelCosts(CostsModelPath)) {
      errs() << "Error loading model cost\n";
      return err.value();
    }
  }
  if (ScramblingCostsPath.length() > 0) {
    if (auto err = LoadScramblingCost(ScramblingCostsPath)) {
      errs() << "Error loading scrambling cost\n";
      return err.value();
    }
  }
  if (ConfigPath.length() > 0) {
    if (auto err = LoadConfig(ConfigPath)) {
      errs() << "Error loading config\n";
      return err.value();
    }
  }

  ReconfigureStaff(Judges, Scramblers, Runners, CubesPerRunner);
  ReconfigureRound(Cutoff, TimeLimit);
  ReconfigureStats(ExtraRate, MiscrambleRate);

  EmitConfig(std::cout);

  SimuGroup(EventId, GroupSize, Avg, ModelId);

  return 0;
}
