#include "libsimu.hpp"

#include <llvm/ObjectYAML/ObjectYAML.h>

namespace libsimu::programs {

struct ModelConfig : libsimu::PropertiesMap {
};

struct SetupConfig : libsimu::PropertiesMap {
};

struct ScramblingConfig : libsimu::PropertiesMap {
};

struct Config {
  ModelConfig Model;
  SetupConfig Setup;
  ScramblingConfig Scrambling;
};

struct ConfigRet {
  ErrCodeTy Err;
  Config C;
};

ConfigRet loadConfig(std::string const &Filename);

ErrCodeTy loadPsychsheet(std::string const &Filename, std::vector<Time> &Times);

}

namespace libsimu {
  // Defined in libsimu
  void emitConfig();
}

#define PROP(EntryName, ModelName)\
  io.mapOptional(#EntryName, C[#EntryName]);
template <>
struct llvm::yaml::MappingTraits<libsimu::programs::ModelConfig> {
  static void mapping(IO &io, libsimu::programs::ModelConfig &C) {
#include "model_props.def"
  }
};

template <>
struct llvm::yaml::MappingTraits<libsimu::programs::SetupConfig> {
  static void mapping(IO &io, libsimu::programs::SetupConfig &C) {
#include "setup_props.def"
  }
};
#undef PROP

template <>
struct llvm::yaml::MappingTraits<libsimu::programs::ScramblingConfig> {
  static void mapping(IO &io, libsimu::programs::ScramblingConfig &SC) {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
    io.mapOptional(#Id, SC[#Id]);
#include "events.def"
  }
};


template <>
struct llvm::yaml::MappingTraits<libsimu::programs::Config> {
  static void mapping(IO &io, libsimu::programs::Config &C) {
    io.mapRequired("setup", C.Setup);
    io.mapOptional("model", C.Model);
    io.mapOptional("scrambling", C.Scrambling);
  }
};

