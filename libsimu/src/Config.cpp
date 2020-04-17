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
    ErrCodeTy Ret(errors::SUCCESS);
    // FIXME: build a diff between Props keys and allowed ones to emit error.
#include "model_props.def"
    return Ret;
  }

  ErrCodeTy Setup::loadMap(PropertiesMap const &Props) {
    ErrCodeTy Ret(errors::SUCCESS);
    // FIXME: build a diff between Props keys and allowed ones to emit error.
#include "setup_props.def"
    return Ret;
  }
#undef PROP

  ErrCodeTy Scrambling::loadMap(PropertiesMap const &Props) {
    ErrCodeTy Ret(errors::SUCCESS);
    string id;
    // FIXME: build a diff between Props keys and allowed ones to emit error.
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime)\
    id = string{#Id};\
    if (Props.count(id))\
      at(id) = Props.at(id);
#include "events.def"
    return Ret;
  }

  ErrCodeTy loadConfig(PropertiesMap const &Setup, PropertiesMap const &Model,
    PropertiesMap const &Scrambling)
  {
    if (Setup::get().loadMap(Setup)) {
      return errors::INVALID_CONFIG;
    }
    if (Model::get().loadMap(Model)) {
      return errors::INVALID_CONFIG;
    }
    if (Scrambling::get().loadMap(Scrambling)) {
      return errors::INVALID_CONFIG;
    }
    return errors::SUCCESS;
  }

#define PROP(EntryName, MemberName)\
  Props[#EntryName] = MemberName;
  PropertiesMap Model::asMap() const {
    PropertiesMap Props;
#include "model_props.def"
    return Props;
  }

  PropertiesMap Setup::asMap() const {
    PropertiesMap Props;
#include "setup_props.def"
    return Props;
  }
#undef PROP

  PropertiesMap getModelProps() {
    return Model::get().asMap();
  }

  PropertiesMap getSetupProps() {
    return Setup::cget().asMap();
  }

  PropertiesMap getScramblingProps() {
    return Scrambling::get();
  }
}
