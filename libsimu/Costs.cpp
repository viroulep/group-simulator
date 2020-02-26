#include "libsimu.hpp"
#include "Costs.hpp"
#include "Actors.hpp"
#include "WCAEvent.hpp"

using namespace std;
using namespace llvm;

namespace libsimu {

ModelCosts &ModelCosts::get()
{
  static ModelCosts MC;
  return MC;
}

ScramblingCosts::ScramblingCosts() {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank) \
  (*this)[#Id] = 0;
#include "events.def"
}

ScramblingCosts &ScramblingCosts::get()
{
  static ScramblingCosts SC;
  return SC;
}

template<typename T>
error_code LoadCosts(const string &Filename)
{
  ErrorOr<unique_ptr<MemoryBuffer>> Buff = MemoryBuffer::getFile(Filename);
  if (!Buff) {
    return Buff.getError();
  }
  yaml::Input YIn(Buff.get()->getBuffer());
  YIn >> T::get();
  return YIn.error();
}

error_code LoadModelCosts(const string &Filename)
{
  return LoadCosts<ModelCosts>(Filename);
}

error_code LoadScramblingCost(const string &Filename)
{
  return LoadCosts<ScramblingCosts>(Filename);
}

error_code LoadConfig(const string &Filename)
{
  return LoadCosts<Config>(Filename);
}

}
