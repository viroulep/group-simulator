#include <iostream>
#include <map>

#include "libsimu.hpp"

#include <emscripten/bind.h>


using namespace emscripten;
using namespace libsimu;


EMSCRIPTEN_BINDINGS(my_module) {
  enum_<errors::ErrorKind>("ErrorKind")
    .value("SUCCESS", errors::SUCCESS)
    .value("GENERIC", errors::GENERIC)
    .value("SIMULATION_FAILURE", errors::SIMULATION_FAILURE)
    .value("INVALID_EVENT", errors::INVALID_EVENT)
    .value("INVALID_CONFIG", errors::INVALID_CONFIG)
    .value("INVALID_FILE", errors::INVALID_FILE)
    ;
  function("errorMessage", &errors::errorMessage);
  register_map<std::string, unsigned long>("MapStringInt");
  register_vector<Time>("VectorTime");
  // Because for map.keys()
  register_vector<std::string>("VectorString");
  constant("DEFAULT_MODEL", DefaultSimulator);
  value_object<TimeResult>("TimeResult")
    .field("Err", &TimeResult::Err)
    .field("Value", &TimeResult::Value)
    ;
  value_object<OptResult>("OptResult")
    .field("Err", &OptResult::Err)
    .field("BestResult", &OptResult::BestResult)
    .field("Judges", &OptResult::Judges)
    .field("Runners", &OptResult::Runners)
    .field("Scramblers", &OptResult::Scramblers)
    ;
  function("loadConfig", &loadConfig);
  function("getSetupProps", &getSetupProps);
  function("getModelProps", &getModelProps);
  function("getScramblingProps", &getScramblingProps);
  function("reconfigureStaff", &reconfigureStaff);
  function("reconfigureRound", &reconfigureRound);
  function("reconfigureStats", &reconfigureStats);
  function("simuGroup", &simuGroup);
  function("optimizeStaff", &optimizeStaff);
}
