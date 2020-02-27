#ifndef COSTS_HPP
#define COSTS_HPP

#include <llvm/ObjectYAML/ObjectYAML.h>
#include <llvm/ADT/IndexedMap.h>
#include <iostream>
#include "libsimu.hpp"
#include "WCAEvent.hpp"

namespace libsimu {

struct ModelCosts {
  Time InitGroup = 120;
  Time RunIn = 25;
  Time CompetitorReady = 30;
  Time CompetitorCleanup = 15;
  Time RunOut = 20;
  Time ShutdownGroup = 0;
  // Or CubesPerJudge if no runner
  unsigned int CubesPerRunner = 1;

  friend std::ostream &operator<<(std::ostream &out, const ModelCosts &CM);

  static ModelCosts &get();
private:
  ModelCosts() = default;
};

struct ScramblingCosts : public llvm::StringMap<Time> {
  static ScramblingCosts &get();
  friend std::ostream &operator<<(std::ostream &out, const ScramblingCosts &CM);
private:
  ScramblingCosts();
};

}

template <>
struct llvm::yaml::MappingTraits<libsimu::ModelCosts> {
  static void mapping(IO &io, libsimu::ModelCosts &model) {
    io.mapOptional("init_group", model.InitGroup);
    io.mapOptional("run_in", model.RunIn);
    io.mapOptional("competitor_ready", model.CompetitorReady);
    io.mapOptional("competitor_cleanup", model.CompetitorCleanup);
    io.mapOptional("run_out", model.RunOut);
    io.mapOptional("shutdown_group", model.ShutdownGroup);
  }
};

template <>
struct llvm::yaml::MappingTraits<libsimu::ScramblingCosts> {
  static void mapping(IO &io, libsimu::ScramblingCosts &SC) {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
    io.mapOptional(#Id, SC[#Id]);
#include "events.def"
  }
};
#endif
