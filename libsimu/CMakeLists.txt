
set(LIBSIMU_SOURCES
  Costs.cpp
  Debug.cpp
  Actors.cpp
  WCAEvent.cpp
  GroupSimulator.cpp
  Run.cpp
  )

add_library(libsimu ${LIBSIMU_SOURCES})
llvm_config(libsimu Support)
