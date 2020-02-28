
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

add_library(libsimu ${LIBSIMU_SOURCES})
# We create a static lib but it needs to be PIC for the native node module
# (I can't believe I'm writing about node module in a C++ cmake file...)
target_compile_options(libsimu PRIVATE "-fPIC")
llvm_config(libsimu Support)
