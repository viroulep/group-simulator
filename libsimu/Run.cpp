#include "libsimu.hpp"
#include "WCAEvent.hpp"
#include "GroupSimulator.hpp"
#include "Config.hpp"
#include "Debug.hpp"

using namespace std;
namespace libsimu {

const string DefaultSimulator = "Runners";

ErrCodeTy ReconfigureStaff(uint8_t Judges, uint8_t Scramblers,
    uint8_t Runners)
{
  Setup &C = Setup::get();
  if (Judges) {
    C.Judges = Judges;
  }
  if (Scramblers) {
    C.Scramblers = Scramblers;
  }
  if (Runners) {
    C.Runners = Runners;
  }
  return 0;
}

ErrCodeTy ReconfigureRound(Time Cutoff, Time TimeLimit /*= 600 */)
{
  Setup &C = Setup::get();
  C.Cutoff = Cutoff;
  C.TimeLimit = TimeLimit;
  return 0;
}

ErrCodeTy ReconfigureStats(uint8_t ExtraRate, uint8_t MiscrambleRate)
{
  Setup &C = Setup::get();
  C.ExtraRate = ExtraRate;
  C.MiscrambleRate = MiscrambleRate;
  return 0;
}

TimeResult SimuGroup(const string &EventId, const vector<Time> &Times,
    const std::string &ModelId /*= DefaultSimulator */)
{
  WCAEvent::WCAEventKind K = WCAEvent::WCAEventIdToKind(EventId);
  if (K == WCAEvent::E_Unknown) {
    return { 1, 0 };
  }

  WCAEvent &Ev = WCAEvent::Get(K);
  unique_ptr<GroupSimulator> Simu = GroupSimulator::Create(ModelId, Ev, Times);
  // FIXME: emit config
  // FIXME: do this only if debug
  cout << "Simu {\n";
  cout << "  Event: " << Ev.Name << "\n";
  cout << "  GroupSize: " << Times.size() << "\n";
  cout << "  Times: " << Times << "\n";
  cout << "}\n";

  // event loop
  return Simu->Run();
}

OptResult OptimizeStaff(const string &EventId,
  const vector<Time> &Times, uint8_t MaxJudges, uint8_t TotalStaff,
  const string &ModelId /* = DefaultSimulator */)
{
  WCAEvent::WCAEventKind K = WCAEvent::WCAEventIdToKind(EventId);
  if (K == WCAEvent::E_Unknown) {
    return { 1, 0 };
  }

  WCAEvent &Ev = WCAEvent::Get(K);
  static constexpr uint8_t MinScramblers = 1;
  uint8_t MinRunners = 0;
  // FIXME: ugly
  if (GroupSimulator::ModelUsesRunners(ModelId))
    MinRunners = 1;

  uint8_t MinJudges = MaxJudges / 2;

  if (MaxJudges > TotalStaff - MinScramblers - MinRunners) {
    cerr << "MaxJudges needs to leave space for scramblers and runners\n";
  }
  OptResult Res = { 0 };

  Res.BestResult = std::numeric_limits<decltype(OptResult::BestResult)>::max();
  Res.Scramblers = std::numeric_limits<decltype(OptResult::Scramblers)>::max();

  for (uint8_t J = MinJudges; J <= MaxJudges; J++) {
    uint8_t RemainingStaff = TotalStaff - J;
    for (uint8_t S = MinScramblers; S <= RemainingStaff - MinRunners; S++) {
      uint8_t R = RemainingStaff - S;
      ReconfigureStaff(J, S, R);
      unique_ptr<GroupSimulator> Simu = GroupSimulator::Create(ModelId, Ev, Times);
      if (!GroupSimulator::ModelUsesRunners(ModelId)) {
        R = 0;
      }
      TimeResult ResultForConfig = Simu->Run();
      if (ResultForConfig.Err) {
        return { ResultForConfig.Err, 0 };
      }
      Time Duration = ResultForConfig.Value;
      // Update the minimum if needed, for identical times, less scramblers wins
      if (tie(Duration, S) < tie(Res.BestResult, Res.Scramblers)) {
        Res.BestResult = Duration;
        Res.Judges = J;
        Res.Scramblers = S;
        Res.Runners = R;
      }
    }
  }

  return Res;
}

}
