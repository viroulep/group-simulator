#include "libsimu.hpp"
#include "Config.hpp"

using namespace std;
using namespace llvm;

namespace libsimu {
  Setup &Setup::get() {
    static Setup C;
    return C;
  }

  Model &Model::get()
  {
    static Model MC;
    return MC;
  }

  Scrambling::Scrambling() {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
    (*this)[#Id] = DefaultScramblingTime;
#include "events.def"
  }

  Scrambling &Scrambling::get()
  {
    static Scrambling SC;
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
    return LoadCosts<Model>(Filename);
  }

  error_code LoadScramblingCost(const string &Filename)
  {
    return LoadCosts<Scrambling>(Filename);
  }

  error_code LoadConfig(const string &Filename)
  {
    return LoadCosts<Setup>(Filename);
  }

}
