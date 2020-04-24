#include <map>
#include <cassert>
#include "Config.hpp"
#include "WCAEvent.hpp"

using namespace std;

namespace libsimu {

namespace {
  map<WCAEvent::WCAEventKind, unique_ptr<WCAEvent>> WCAEventMap;
}

WCAEvent::WCAEvent(WCAEventKind K, string &&Id, string &&Name, uint8_t Max, uint8_t Cutoff, uint8_t Rank) :
  MaxAttempts(Max), CutoffAttempts(Cutoff), Rank(Rank), Id(Id), Name(Name), Kind(K) {}

WCAEvent::WCAEventKind WCAEvent::WCAEventIdToKind(const string &EventId)
{
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
  if (EventId == #Id) {\
    return E_##Id;\
  } else
#include "events.def"
  {
    return E_Unknown;
  }
}

Time WCAEvent::getScramblingCost() const
{
  return Scrambling::get()[Id];
}

bool WCAEvent::hasCumulativeTimeLimit() const
{
  // Best practice is to use cumulative time limits for blindfolded events.
  assert(Kind != E_Unknown);
  switch(Kind) {
    case E_444bf: // Intentional fall-through!
    case E_333bf:
    case E_555bf:
      return true;
    default:
      return false;
  }
}

unique_ptr<WCAEvent> WCAEvent::Create(WCAEventKind K)
{
  assert(K != E_Unknown);
  switch (K) {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
    case E_##Id: \
      return std::make_unique<WCAEvent##Id>();
#include "events.def"
    case E_Unknown:
      break;
  }
  return nullptr;
}

WCAEvent &WCAEvent::Get(WCAEventKind K)
{
  assert(K != E_Unknown);
  if (!WCAEventMap[K])
    WCAEventMap[K] = Create(K);
  return *WCAEventMap[K].get();
}

}
