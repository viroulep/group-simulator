#include "libsimu.hpp"
#include "WCAEvent.hpp"
#include "GroupSimulator.hpp"
#include "Config.hpp"
#include "Debug.hpp"

using namespace std;
namespace libsimu {
namespace errors {

string errorMessage(ErrorKind K)
{
  switch (K) {
    case SUCCESS:
      return "Operation completed successfully";
    case GENERIC:
      return "There was an error processing your request";
    case SIMULATION_FAILURE:
      return "The simulation failed";
    case INVALID_EVENT:
      return "Invalid event supplied";
    case INVALID_CONFIG:
      return "Invalid configuration supplied";
    case INVALID_FILE:
      return "Invalid file supplied";
  }
  return "Unknown";
}

} // errors

const string DefaultSimulator = "Runners";

ErrCodeTy reconfigureStaff(unsigned Judges, unsigned Scramblers,
    unsigned Runners)
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
  return errors::SUCCESS;
}

ErrCodeTy reconfigureRound(Time Cutoff, Time TimeLimit /*= 600 */)
{
  Setup &C = Setup::get();
  C.Cutoff = Cutoff;
  C.TimeLimit = TimeLimit;
  return errors::SUCCESS;
}

ErrCodeTy reconfigureStats(unsigned ExtraRate, unsigned MiscrambleRate)
{
  Setup &C = Setup::get();
  C.ExtraRate = ExtraRate;
  C.MiscrambleRate = MiscrambleRate;
  return errors::SUCCESS;
}

TimeResult simuGroup(const string &EventId, const TimeVector &Times,
    const std::string &ModelId /*= DefaultSimulator */)
{
  WCAEvent::WCAEventKind K = WCAEvent::WCAEventIdToKind(EventId);
  if (K == WCAEvent::E_Unknown) {
    return { errors::INVALID_EVENT, 0 };
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

OptResult optimizeStaff(const string &EventId,
  const TimeVector &Times, unsigned MaxJudges, unsigned TotalStaff,
  const string &ModelId /* = DefaultSimulator */)
{
  WCAEvent::WCAEventKind K = WCAEvent::WCAEventIdToKind(EventId);
  if (K == WCAEvent::E_Unknown) {
    return { errors::INVALID_EVENT, 0 };
  }

  WCAEvent &Ev = WCAEvent::Get(K);
  static constexpr unsigned MinScramblers = 1;
  unsigned MinRunners = 0;
  // FIXME: ugly
  if (GroupSimulator::ModelUsesRunners(ModelId))
    MinRunners = 1;

  unsigned MinJudges = MaxJudges / 2;

  if (MaxJudges > TotalStaff - MinScramblers - MinRunners) {
    cerr << "MaxJudges needs to leave space for scramblers and runners\n";
  }
  OptResult Res = { errors::SUCCESS };

  Res.BestResult = std::numeric_limits<decltype(OptResult::BestResult)>::max();
  Res.Scramblers = std::numeric_limits<decltype(OptResult::Scramblers)>::max();

  for (unsigned J = MinJudges; J <= MaxJudges; J++) {
    unsigned RemainingStaff = TotalStaff - J;
    for (unsigned S = MinScramblers; S <= RemainingStaff - MinRunners; S++) {
      unsigned R = RemainingStaff - S;
      reconfigureStaff(J, S, R);
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
