#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <llvm/ObjectYAML/ObjectYAML.h>

#include "libsimu.hpp"

namespace libsimu {

// Proxy class to (re)-fill J/R/S static data.
// May contain more configuration later.
struct Setup {
  static Setup &get();
  static const Time MaxTimeLimit = 600;
  uint8_t Judges = 10;
  uint8_t Runners = 2;
  uint8_t Scramblers = 3;
  // Between 0 and 100, 0 means no extra can occur
  uint8_t ExtraRate = 0;
  // The maximum of cubes a runner can take
  uint8_t MaxCubes = 3;
  // Between 0 and 100, 0 means no miscramble
  uint8_t MiscrambleRate = 0;
  // FIXME: move this to simuparams
  Time TimeLimit = MaxTimeLimit;
  Time Cutoff = 600;
  friend std::ostream &operator<<(std::ostream &out, const Setup &C);
private:
  Setup() = default;
};

struct Model {
  Time InitGroup = 120;
  Time RunIn = 25;
  Time CompetitorReady = 30;
  Time CompetitorCleanup = 15;
  Time RunOut = 20;
  Time ShutdownGroup = 0;
  // Or CubesPerJudge if no runner
  unsigned int CubesPerRunner = 1;

  friend std::ostream &operator<<(std::ostream &out, const Model &CM);

  static Model &get();
private:
  Model() = default;
};

struct Scrambling : public llvm::StringMap<Time> {
  static Scrambling &get();
  friend std::ostream &operator<<(std::ostream &out, const Scrambling &CM);
private:
  Scrambling();
};

}

template <>
struct llvm::yaml::MappingTraits<libsimu::Model> {
  static void mapping(IO &io, libsimu::Model &model) {
    io.mapOptional("init_group", model.InitGroup);
    io.mapOptional("run_in", model.RunIn);
    io.mapOptional("competitor_ready", model.CompetitorReady);
    io.mapOptional("competitor_cleanup", model.CompetitorCleanup);
    io.mapOptional("run_out", model.RunOut);
    io.mapOptional("shutdown_group", model.ShutdownGroup);
  }
};

template <>
struct llvm::yaml::MappingTraits<libsimu::Scrambling> {
  static void mapping(IO &io, libsimu::Scrambling &SC) {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
    io.mapOptional(#Id, SC[#Id]);
#include "events.def"
  }
};

template <>
struct llvm::yaml::MappingTraits<libsimu::Setup> {
  static void mapping(IO &io, libsimu::Setup &C) {
    io.mapOptional("extra_rate", C.ExtraRate);
    io.mapOptional("cubes_per_runner", C.MaxCubes);
    io.mapOptional("miscramble_rate", C.MiscrambleRate);
    io.mapOptional("judges", C.Judges);
    io.mapOptional("runners", C.Runners);
    io.mapOptional("scramblers", C.Scramblers);
    io.mapOptional("time_limit", C.TimeLimit);
    io.mapOptional("cutoff", C.Cutoff);
  }
};

#endif
