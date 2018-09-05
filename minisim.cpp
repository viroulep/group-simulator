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

using namespace std;




int main()
{
  GroupSimulator *simu = new RunnerSystemSimulator(20, 10, 4, 3);
  ScramblingCosts costs = ScramblingCosts("./costs/events.yml");

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
  cout << "Round took " << durationInMinute.count() << " minutes and " << remaining.count() << " seconds.\n";
  delete simu;
  return 0;
}
