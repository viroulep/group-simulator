
set(LIBSIMU_SOURCES
  Actors.cpp
  Costs.cpp
  Cube.cpp
  Debug.cpp
  GroupSimulator.cpp
  JudgesRunSimulator.cpp
  RunnerSystemSimulator.cpp
  Run.cpp
  WCAEvent.cpp
  )

add_library(libsimu SHARED ${LIBSIMU_SOURCES})
llvm_config(libsimu Support)
