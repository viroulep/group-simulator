#ifndef COSTS_MODEL_HPP
#define COSTS_MODEL_HPP

#include <llvm/ObjectYAML/ObjectYAML.h>
#include <iostream>

struct CostsModel {
  double InitGroup = 0;
  double RunIn = 0;
  double CompetitorReady = 0;
  double CompetitorCleanup = 0;
  double RunOut = 0;
  double ShutdownGroup = 0;
  // Or CubesPerJudge if no runner
  double CubesPerRunner = 1;

  friend std::ostream &operator<<(std::ostream &out, const CostsModel &CM);

  static CostsModel *get();
private:
  CostsModel() {};
};

template <>
struct llvm::yaml::MappingTraits<CostsModel> {
  static void mapping(IO &io, CostsModel &model) {
    io.mapOptional("init_group", model.InitGroup);
    io.mapOptional("run_in", model.RunIn);
    io.mapOptional("competitor_ready", model.CompetitorReady);
    io.mapOptional("competitor_cleanup", model.CompetitorCleanup);
    io.mapOptional("run_out", model.RunIn);
    io.mapOptional("shutdown_group", model.ShutdownGroup);
    io.mapOptional("cubes_per_runner", model.CubesPerRunner);
  }
};

#endif
