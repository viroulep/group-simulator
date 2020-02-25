#ifndef COSTS_HPP
#define COSTS_HPP

#include <llvm/ObjectYAML/ObjectYAML.h>
#include <llvm/ADT/IndexedMap.h>
#include <iostream>
#include "libsimu.hpp"
#include "WCAEvent.hpp"

struct ModelCosts {
  Time InitGroup = 0;
  Time RunIn = 0;
  Time CompetitorReady = 0;
  Time CompetitorCleanup = 0;
  Time RunOut = 0;
  Time ShutdownGroup = 0;
  // Or CubesPerJudge if no runner
  unsigned int CubesPerRunner = 1;

  friend std::ostream &operator<<(std::ostream &out, const ModelCosts &CM);

  static ModelCosts *get();
private:
  ModelCosts() {};
};

struct ScramblingCosts : public llvm::StringMap<Time> {
  static ScramblingCosts *get();
  friend std::ostream &operator<<(std::ostream &out, const ScramblingCosts &CM);
private:
  ScramblingCosts();
};

template <>
struct llvm::yaml::MappingTraits<ModelCosts> {
  static void mapping(IO &io, ModelCosts &model) {
    io.mapOptional("init_group", model.InitGroup);
    io.mapOptional("run_in", model.RunIn);
    io.mapOptional("competitor_ready", model.CompetitorReady);
    io.mapOptional("competitor_cleanup", model.CompetitorCleanup);
    io.mapOptional("run_out", model.RunIn);
    io.mapOptional("shutdown_group", model.ShutdownGroup);
    io.mapOptional("cubes_per_runner", model.CubesPerRunner);
  }
};

template <>
struct llvm::yaml::MappingTraits<ScramblingCosts> {
  static void mapping(IO &io, ScramblingCosts &SC) {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank) \
    io.mapOptional(#Id, SC[#Id]);
#include "events.def"
  }
};

#endif
