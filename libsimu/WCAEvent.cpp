#include <llvm/ADT/StringSwitch.h>
#include <map>
#include "Costs.hpp"
#include "WCAEvent.hpp"

using namespace std;
using namespace llvm;

namespace libsimu {

namespace {
  map<WCAEvent::WCAEventKind, unique_ptr<WCAEvent>> WCAEventMap;
}

WCAEvent::WCAEvent(WCAEventKind K, string &&Id, string &&Name, uint8_t Max, uint8_t Cutoff, uint8_t Rank) :
  MaxAttempts(Max), CutoffAttempts(Cutoff), Rank(Rank), Id(Id), Name(Name), Kind(K) {}

WCAEvent::WCAEventKind WCAEvent::WCAEventIdToKind(const string &Id)
{
  return StringSwitch<WCAEventKind>(Id)
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
    .Case(#Id, E_##Id)
#include "events.def"
    .Default(E_Unknown);
}

Time WCAEvent::ScramblingCost() const
{
  return ScramblingCosts::get()[Id];
}

Expected<unique_ptr<WCAEvent>> WCAEvent::Create(WCAEventKind K)
{
  switch (K) {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
    case E_##Id: \
      return std::make_unique<WCAEvent##Id>();
#include "events.def"
    case E_Unknown:
      break;
  }

  return make_error<StringError>("Unknown WCA Event", make_error_code(errc::invalid_argument));
}

WCAEvent &WCAEvent::Get(const std::string &Id)
{
  return Get(WCAEventIdToKind(Id));
}

WCAEvent &WCAEvent::Get(WCAEventKind K)
{
  if (!WCAEventMap[K])
    WCAEventMap[K] = ExitOnError{}(Create(K));
  return *WCAEventMap[K].get();
}

}
