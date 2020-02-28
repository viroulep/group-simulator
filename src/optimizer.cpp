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
static cl::opt<std::string> CostsModelPath("mcosts", cl::desc("Specify path to model costs"), cl::value_desc("filename"));
static cl::opt<std::string> ScramblingCostsPath("scosts", cl::desc("Specify path to scrambling costs"), cl::value_desc("filename"));
static cl::opt<std::string> ConfigPath("config", cl::desc("Specify path to config"), cl::value_desc("filename"));
static cl::opt<std::string> Psychsheet("list", cl::desc("Specify path to the list of times"), cl::value_desc("filename"));

static cl::opt<std::string> ModelId("m", cl::desc("Set the simulator to use (Runners, JudgesRun)"), cl::value_desc("model_id"), cl::init("Runners"));

static cl::opt<unsigned> Judges("j", cl::desc("Set the max number of judges"), cl::value_desc("judges"), cl::init(10));
static cl::opt<unsigned> Staff("staff", cl::desc("Set the number of staff"), cl::value_desc("runners"), cl::init(2));

static cl::opt<unsigned> GroupSize("size", cl::desc("Set the group size"), cl::value_desc("number"), cl::init(20));
static cl::opt<unsigned> Avg("avg-time", cl::desc("Set the group average time"), cl::value_desc("seconds"), cl::init(10));


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

  //EmitConfig(std::cout);

  std::vector<Time> Times(GroupSize, Avg);

  OptResult Result{0};

  OptimizeStaff(&Result, EventId, Times, Judges, Staff, ModelId);

  chrono::seconds roundDuration(Result.BestResult);
  chrono::minutes durationInMinute = chrono::duration_cast<chrono::minutes>(roundDuration);
  chrono::seconds remaining = roundDuration - durationInMinute;
  cout << "Best group took " << durationInMinute.count() << " minutes and " << remaining.count() << " seconds.\n";
  cout << "Configuration {\n";
  cout << "  Judges: " << to_string(Result.Judges) << "\n";
  cout << "  Scramblers: " << to_string(Result.Scramblers) << "\n";
  cout << "  Runners: " << to_string(Result.Runners) << "\n";
  cout << "}\n";

  return 0;
}
