set(LIBSIMU_SOURCES
  Config.cpp
  Cube.cpp
  Debug.cpp
  GroupSimulator.cpp
  JudgesRunSimulator.cpp
  RunnerSystemSimulator.cpp
  Run.cpp
  RNG.cpp
  WCAEvent.cpp
  )

add_library(libsimu ${LIBSIMU_SOURCES})
