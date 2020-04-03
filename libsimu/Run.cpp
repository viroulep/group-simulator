#include "libsimu.hpp"
#include "WCAEvent.hpp"
#include "GroupSimulator.hpp"
#include "Config.hpp"
#include "Debug.hpp"

using namespace std;
namespace libsimu {

const string DefaultSimulator = "Runners";

error_code ReconfigureStaff(uint8_t Judges, uint8_t Scramblers,
    uint8_t Runners, uint8_t CubesPerRunner)
{
  // TODO check stuff
  Setup &C = Setup::get();
  C.Judges = Judges;
  C.Scramblers = Scramblers;
  C.Runners = Runners;
  C.MaxCubes = CubesPerRunner;
  return error_code{};
}

error_code ReconfigureRound(Time Cutoff, Time TimeLimit /*= 600 */)
{
  Setup &C = Setup::get();
  C.Cutoff = Cutoff;
  C.TimeLimit = TimeLimit;
  return error_code{};
}

error_code ReconfigureStats(uint8_t ExtraRate, uint8_t MiscrambleRate)
{
  Setup &C = Setup::get();
  C.ExtraRate = ExtraRate;
  C.MiscrambleRate = MiscrambleRate;
  return error_code{};
}

error_code SimuGroup(Time *Result, const string &EventId, const vector<Time> &Times,
    const std::string &ModelId /*= DefaultSimulator */)
{
  WCAEvent &Ev = WCAEvent::Get(EventId);
  unique_ptr<GroupSimulator> Simu = GroupSimulator::Create(ModelId, Ev, Times);
  // FIXME: emit config
  // FIXME: do this only if debug
  cout << "Simu {\n";
  cout << "  Event: " << Ev.Name << "\n";
  cout << "  GroupSize: " << Times.size() << "\n";
  cout << "  Times: " << Times << "\n";
  cout << "}\n";

  // event loop
  *Result = Simu->Run();

  return error_code{};
}

error_code OptimizeStaff(OptResult *Res, const string &EventId,
  const vector<Time> &Times, uint8_t MaxJudges, uint8_t TotalStaff,
  const string &ModelId /* = DefaultSimulator */)
{
  WCAEvent &Ev = WCAEvent::Get(EventId);
  static constexpr uint8_t MinScramblers = 1;
  uint8_t MinRunners = 0;
  // FIXME: ugly
  if (GroupSimulator::ModelUsesRunners(ModelId))
    MinRunners = 1;

  uint8_t MinJudges = MaxJudges / 2;

  if (MaxJudges > TotalStaff - MinScramblers - MinRunners) {
    cerr << "MaxJudges needs to leave space for scramblers and runners\n";
  }

  Res->BestResult = std::numeric_limits<decltype(OptResult::BestResult)>::max();
  Res->Scramblers = std::numeric_limits<decltype(OptResult::Scramblers)>::max();
  Setup &C = Setup::get();

  for (uint8_t J = MinJudges; J <= MaxJudges; J++) {
    uint8_t RemainingStaff = TotalStaff - J;
    for (uint8_t S = MinScramblers; S <= RemainingStaff - MinRunners; S++) {
      uint8_t R = RemainingStaff - S;
      ReconfigureStaff(J, S, R, C.MaxCubes);
      unique_ptr<GroupSimulator> Simu = GroupSimulator::Create(ModelId, Ev, Times);
      if (!GroupSimulator::ModelUsesRunners(ModelId)) {
        R = 0;
      }
      Time Duration = Simu->Run();
      // Update the minimum if needed, for identical times, less scramblers wins
      if (tie(Duration, S) < tie(Res->BestResult, Res->Scramblers)) {
        Res->BestResult = Duration;
        Res->Judges = J;
        Res->Scramblers = S;
        Res->Runners = R;
      }
    }
  }


  return error_code{};
}

}
