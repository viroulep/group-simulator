#include "libsimu.hpp"
#include "Config.hpp"

using namespace std;

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

#define PROP(EntryName, MemberName)\
    if (Props.count(#EntryName))\
      MemberName = Props.at(#EntryName);
  ErrCodeTy Model::loadMap(PropertiesMap const &Props) {
    ErrCodeTy Ret = 0;
    // FIXME: build a diff between Props keys and allowed ones to emit error.
#include "model_props.def"
    return Ret;
  }

  ErrCodeTy Setup::loadMap(PropertiesMap const &Props) {
    ErrCodeTy Ret = 0;
    // FIXME: build a diff between Props keys and allowed ones to emit error.
#include "setup_props.def"
    return Ret;
  }
#undef PROP

  ErrCodeTy Scrambling::loadMap(PropertiesMap const &Props) {
    ErrCodeTy Ret = 0;
    string id;
    // FIXME: build a diff between Props keys and allowed ones to emit error.
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime)\
    id = string{#Id};\
    if (Props.count(id))\
      at(id) = Props.at(id);
#include "events.def"
    return Ret;
  }

  ErrCodeTy LoadConfig(PropertiesMap const &Setup, PropertiesMap const &Model,
    PropertiesMap const &Scrambling)
  {
    if (Setup::get().loadMap(Setup)) {
      return 1;
    }
    if (Model::get().loadMap(Model)) {
      return 1;
    }
    if (Scrambling::get().loadMap(Scrambling)) {
      return 1;
    }
    return 0;
#if 0
    ErrorOr<unique_ptr<MemoryBuffer>> Buff = MemoryBuffer::getFile(Filename);
    if (!Buff) {
      return Buff.getError();
    }
    yaml::Input YIn(Buff.get()->getBuffer());
    Config C;
    YIn >> C;
    return YIn.error();
#endif
  }
}
