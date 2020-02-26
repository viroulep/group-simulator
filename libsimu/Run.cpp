#include "libsimu.hpp"
#include "WCAEvent.hpp"
#include "GroupSimulator.hpp"

using namespace std;
namespace libsimu {

error_code SimuGroup(const string &EventId, unsigned int N)
{
  WCAEvent &Ev = WCAEvent::Get(EventId);
  CubeSet cubes;
  cout << Ev;

  for (unsigned int i = 0; i < N; i++) {
    cubes.insert(new Cube(12));
  }

  unique_ptr<RunnerSystemSimulator> Simu = make_unique<RunnerSystemSimulator>(Ev, cubes);

  // event loop
  while (!Simu->Done()) {
    cout << *Simu;
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
