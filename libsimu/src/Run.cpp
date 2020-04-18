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

TimeResult simuGroup(string const &EventId, TimeVector const &Times,
    PropertiesMap const &SetupOverride, std::string const &ModelId)
{
  WCAEvent::WCAEventKind K = WCAEvent::WCAEventIdToKind(EventId);
  if (K == WCAEvent::E_Unknown) {
    return { errors::INVALID_EVENT, 0 };
  }

  WCAEvent &Ev = WCAEvent::Get(K);
  unique_ptr<GroupSimulator> Simu =
    GroupSimulator::Create(ModelId, Ev, Times, SetupOverride);

  // event loop
  return Simu->Run();
}

OptResult optimizeStaff(string const &EventId,
  const TimeVector &Times, unsigned MaxJudges, unsigned TotalStaff,
  PropertiesMap const &SetupOverride, string const &ModelId)
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

  int Leftovers = TotalStaff - MinScramblers - MinRunners;
  if (Leftovers <= 0) {
    cerr << "Not enough TotalStaff\n";
    return { errors::SIMULATION_FAILURE };
  }
  if (MaxJudges > (unsigned)Leftovers) {
    cerr << "MaxJudges needs to leave space for scramblers and runners\n";
    return { errors::SIMULATION_FAILURE };
  }
  OptResult Res = { errors::SUCCESS };

  Res.BestResult = std::numeric_limits<decltype(OptResult::BestResult)>::max();
  Res.Scramblers = std::numeric_limits<decltype(OptResult::Scramblers)>::max();

  // Local object to change the simulator's configuration
  PropertiesMap LocalSetup(SetupOverride);
  // For optimizing we want the best case, we don't want extra/miscramble
  // probabilities to mess with figuring out the correct setup.
  LocalSetup["miscramble_rate"] = 0;
  LocalSetup["extra_rate"] = 0;

  for (unsigned J = MinJudges; J <= MaxJudges; J++) {
    unsigned RemainingStaff = TotalStaff - J;
    for (unsigned S = MinScramblers; S <= RemainingStaff - MinRunners; S++) {
      unsigned R = RemainingStaff - S;
      LocalSetup["judges"] = J;
      LocalSetup["scramblers"] = S;
      LocalSetup["runners"] = R;
      unique_ptr<GroupSimulator> Simu =
        GroupSimulator::Create(ModelId, Ev, Times, LocalSetup);
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
