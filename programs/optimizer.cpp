#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/YAMLTraits.h>
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <cassert>
#include <iterator>

#include "libsimu.hpp"
#include "config_loader.hpp"

using namespace llvm;
using namespace std;
using namespace libsimu;

// Positional and mandatory
static cl::opt<std::string> EventId(cl::Positional, cl::desc("<wca_event_id>"), cl::Required);

// Options
static cl::opt<std::string> ConfigPath("config", cl::desc("Specify path to (maybe partial) config"), cl::value_desc("filename"));
static cl::opt<std::string> Psychsheet("list", cl::desc("Specify path to the list of times"), cl::value_desc("filename"));

static cl::opt<std::string> ModelId("m", cl::desc("Set the simulator to use (Runners, JudgesRun)"), cl::value_desc("model_id"), cl::init("Runners"));

static cl::opt<unsigned> Judges("j", cl::desc("Set the max number of judges"), cl::value_desc("judges"), cl::init(10));
static cl::opt<unsigned> Staff("staff", cl::desc("Set the number of staff"), cl::value_desc("runners"), cl::init(2));

static cl::opt<unsigned> GroupSize("size", cl::desc("Set the group size"), cl::value_desc("number"), cl::init(20));
static cl::opt<unsigned> Avg("avg-time", cl::desc("Set the group average time"), cl::value_desc("seconds"), cl::init(10));


int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);

  if (ConfigPath.length() > 0) {
    auto Res = programs::LoadConfig(ConfigPath);
    if (Res.Err) {
      errs() << "Error loading config\n";
      return Res.Err;
    }
    LoadConfig(Res.C.Setup, Res.C.Model, Res.C.Scrambling);
  }

  //EmitConfig(std::cout);

  std::vector<Time> Times(GroupSize, Avg);

  OptResult Result = OptimizeStaff(EventId, Times, Judges, Staff, ModelId);
  if (Result.Err) {
    cerr << "There was an error during the process :(\n";
    return Result.Err;
  }

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
