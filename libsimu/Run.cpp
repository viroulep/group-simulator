#include "libsimu.hpp"
#include "WCAEvent.hpp"
#include "GroupSimulator.hpp"
#include "Debug.hpp"

using namespace std;
namespace libsimu {

const string DefaultSimulator = "Runners";

error_code ReconfigureStaff(uint8_t Judges, uint8_t Scramblers,
    uint8_t Runners, uint8_t CubesPerRunner)
{
  // TODO check stuff
  Config &C = Config::get();
  C.Judges = Judges;
  C.Scramblers = Scramblers;
  C.Runners = Runners;
  C.MaxCubes = CubesPerRunner;
  return error_code{};
}

error_code ReconfigureRound(Time Cutoff, Time TimeLimit /*= 600 */)
{
  Config &C = Config::get();
  C.Cutoff = Cutoff;
  C.TimeLimit = TimeLimit;
  return error_code{};
}

error_code ReconfigureStats(uint8_t ExtraRate, uint8_t MiscrambleRate)
{
  Config &C = Config::get();
  C.ExtraRate = ExtraRate;
  C.MiscrambleRate = MiscrambleRate;
  return error_code{};
}

error_code SimuGroup(const string &EventId, unsigned int N, Time Avg,
    const std::string &ModelId /*= DefaultSimulator */)
{
  std::vector<Time> cubes(N, Avg);
  return SimuGroup(EventId, cubes, ModelId);
}

error_code SimuGroup(const string &EventId, const std::vector<Time> &Times,
    const std::string &ModelId /*= DefaultSimulator */)
{
  WCAEvent &Ev = WCAEvent::Get(EventId);
  unique_ptr<GroupSimulator> Simu = GroupSimulator::Create(ModelId, Ev, Times);
  // FIXME: do this only if debug
  cout << "Simu {\n";
  cout << "  Event: " << Ev.Name << "\n";
  cout << "  GroupSize: " << Times.size() << "\n";
  cout << "  Times: " << Times << "\n";
  cout << "}\n";

  // event loop
  while (!Simu->Done()) {
    //cout << *Simu;
    //simu->printState();
    SimuEvent currentEvent = Simu->NextEvent();
    if (currentEvent.c) {
      assert(currentEvent.c->attemptsDone >= 0 && currentEvent.c->attemptsDone < 5);
    }
    switch (currentEvent.Kind) {
#define SIMU_EVENT_TYPE(Name)            \
      case SimuEvent::Name:                         \
        Simu->ActOn##Name(currentEvent); \
      break;
#include "types.def"
      case SimuEvent::Unknown:
        cout << "WTF!\n";
    }
    Simu->DoneEvent(currentEvent);
  }

  chrono::seconds roundDuration(Simu->GetWalltime());
  chrono::minutes durationInMinute = chrono::duration_cast<chrono::minutes>(roundDuration);
  chrono::seconds remaining = roundDuration - durationInMinute;
  cout << "Group took " << durationInMinute.count() << " minutes and " << remaining.count() << " seconds.\n";

  return error_code{};
}

}
