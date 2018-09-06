#include<set>
#include<cassert>
#include<vector>
#include<tuple>
#include<iostream>
#include<algorithm>
#include<chrono>
#include<iterator>

#include "types.hpp"
#include "simu.hpp"
#include "costs.hpp"
#include "generator.hpp"
#include "third_party/cxxopts/cxxopts.hpp"

using namespace std;



int main(int argc, char *argv[])
{
  cxxopts::Options options("Group Simulator", "Gives estimate time for a group by simulating it.");
  options.add_options()
    ("cubes", "Input list of cube's time", cxxopts::value<string>())
    ("event", "Event for group", cxxopts::value<string>())
    ("model", "How you will run the group (runner_system|judge_runs_system)", cxxopts::value<string>()->default_value("judge_runs_system"))
    ("scrambling_costs", "Path to the datafile", cxxopts::value<string>()->default_value("./costs/events.yml"))
    ("model_costs", "Path to the model datafile", cxxopts::value<string>()->default_value("./costs/models.yml"))
    ("j,judges", "Number of judges", cxxopts::value<unsigned int>()->default_value("10"))
    ("r,runners", "Number of runners", cxxopts::value<unsigned int>()->default_value("2"))
    ("s,scramblers", "Number of scramblers", cxxopts::value<unsigned int>()->default_value("2"))
    ("c,cutoff", "Cutoff", cxxopts::value<Time>()->default_value("600"))
    ("t,time_limit", "Time limit", cxxopts::value<Time>()->default_value("600"))
    ;

  auto result = options.parse(argc, argv);

  if (result.count("cubes") != 1) {
    cout << "You must provide a file with the list of cubes for the group.\n";
    exit(EXIT_FAILURE);
  }

  if (result.count("event") != 1) {
    cout << "You must provide an event for the group.\n";
    exit(EXIT_FAILURE);
  }

  WCAEventKind k = getWCAEventKindFromId(result["event"].as<string>());
  assert(k != WCAEventUnknown);

  unsigned int judges = result["judges"].as<unsigned int>();
  unsigned int scramblers = result["scramblers"].as<unsigned int>();
  Time cutoff = result["cutoff"].as<Time>();
  Time timeLimit = result["time_limit"].as<Time>();
  string pathToCubes = result["cubes"].as<string>();
  CubeSet cubes = CubeLoader::loadFile(pathToCubes);

  string modelUsed = result["model"].as<string>();

  GroupSimulator *simu;
  if (modelUsed == "runner_system") {
    unsigned int runners = result["runners"].as<unsigned int>();
    simu = new RunnerSystemSimulator(k, cubes, judges, scramblers, runners, cutoff, timeLimit);
  } else if (modelUsed == "judge_runs_system") {
    simu = new JudgeRunsSystemSimulator(k, cubes, judges, scramblers, cutoff, timeLimit);
  } else {
    cout << "Unrecognized model: " << modelUsed << "\n";
    exit(EXIT_FAILURE);
  }


  // event loop
  while (!simu->done()) {
    //simu->printState();
    Event currentEvent = simu->nextEvent();
    if (currentEvent.c) {
      assert(currentEvent.c->attemptsDone >= 0 && currentEvent.c->attemptsDone < 5);
    }
    switch (currentEvent.type) {
#define SIMU_EVENT_TYPE(Name)            \
      case Name:                         \
        simu->actOn##Name(currentEvent); \
      break;
#include "types.def"
      case EventUnknown:
        cout << "WTF!\n";
    }
    simu->doneEvent();
  }
  chrono::seconds roundDuration(simu->walltime());
  chrono::minutes durationInMinute = chrono::duration_cast<chrono::minutes>(roundDuration);
  chrono::seconds remaining = roundDuration - durationInMinute;
  cout << "Group took " << durationInMinute.count() << " minutes and " << remaining.count() << " seconds.\n";
  delete simu;
  return 0;
}
