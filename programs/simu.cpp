#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/YAMLTraits.h>
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <fstream>
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

static cl::opt<unsigned> Judges("j", cl::desc("Overrides the number of judges"), cl::value_desc("judges"));
static cl::opt<unsigned> Scramblers("s", cl::desc("Overrides the number of scramblers"), cl::value_desc("scramblers"));
static cl::opt<unsigned> Runners("r", cl::desc("Overrides the number of runners"), cl::value_desc("runners"));
static cl::opt<unsigned> MaxCubes("max-cubes", cl::desc("Overrides the maximum number of cubes per runner"), cl::value_desc("cubes"));

static cl::opt<unsigned> TimeLimit("tl", cl::desc("Set the time limit"), cl::value_desc("seconds"), cl::init(0));
static cl::opt<unsigned> Cutoff("cut", cl::desc("Set the cutoff"), cl::value_desc("seconds"), cl::init(0));

static cl::opt<unsigned> GroupSize("size", cl::desc("Set the group size"), cl::value_desc("number"), cl::init(20));
static cl::opt<unsigned> Avg("avg-time", cl::desc("Set the group average time"), cl::value_desc("seconds"), cl::init(10));

static cl::opt<unsigned> ExtraRate("extra", cl::desc("Overrides the extra rate"), cl::value_desc("percentage"), cl::init(0));
static cl::opt<unsigned> MiscrambleRate("miscramble", cl::desc("Overrides the miscramble rate"), cl::value_desc("percentage"), cl::init(0));


int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);

  if (ConfigPath.length() > 0) {
    auto Res = programs::loadConfig(ConfigPath);
    if (Res.Err) {
      cerr << errors::errorMessage(Res.Err) << "\n";
      return Res.Err;
    }
    loadConfig(Res.C.Setup, Res.C.Model, Res.C.Scrambling);
  }


  PropertiesMap Override;
  // Default values are 0, and since they are invalid values they are considered
  // as not overriding the config.
#define PROP(EntryName, MemberName)\
  if (MemberName > 0) {\
    Override[#EntryName] = MemberName;\
  }
#include "setup_props.def"
#undef PROP

  std::vector<Time> Times;

  if (Psychsheet.size()) {
    auto Err = programs::loadPsychsheet(Psychsheet, Times);
    if (Err != errors::SUCCESS) {
      return Err;
    }
  } else {
    Times = std::vector<Time>(GroupSize, Avg);
  }

  TimeResult Result = simuGroup(EventId, Times, Override, ModelId);
  if (Result.Err) {
    cerr << "There was an error during the process, see below :(\n";
    cerr << errors::errorMessage(Result.Err) << "\n";
    return Result.Err;
  }

  chrono::seconds roundDuration(Result.Value);
  chrono::minutes durationInMinute = chrono::duration_cast<chrono::minutes>(roundDuration);
  chrono::seconds remaining = roundDuration - durationInMinute;
  cout << "Group took " << durationInMinute.count() << " minutes and " << remaining.count() << " seconds.\n";

  return 0;
}
