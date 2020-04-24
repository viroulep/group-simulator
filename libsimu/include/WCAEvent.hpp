#ifndef WCA_EVENTS_HPP
#define WCA_EVENTS_HPP

#include <memory>
#include "libsimu.hpp"

namespace libsimu {

struct WCAEvent {
  enum WCAEventKind {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
    E_##Id,
#include "events.def"
    E_Unknown,
  };

  WCAEvent(WCAEventKind K, std::string &&Id, std::string &&Name, uint8_t Max, uint8_t Cutoff, uint8_t Rank);
  Time getScramblingCost() const;
  // Yep, not flexible but enough.
  bool hasCumulativeTimeLimit() const;
  uint8_t MaxAttempts;
  uint8_t CutoffAttempts;
  // AFAIK it's under 255
  uint8_t Rank;
  const std::string Id;
  const std::string Name;
  static WCAEvent &Get(WCAEventKind K);
  static WCAEventKind WCAEventIdToKind(const std::string &Id);
  friend std::ostream &operator<<(std::ostream &out, const WCAEvent &Ev);
private:
  static std::unique_ptr<WCAEvent> Create(WCAEventKind K);
  const WCAEventKind Kind;
};




#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
struct WCAEvent##Id : public WCAEvent { \
  WCAEvent##Id() : WCAEvent(E_##Id, #Id, #Name, MaxAttempts, CutoffAttempts, Rank) {} \
};
#include "events.def"

}
#endif
