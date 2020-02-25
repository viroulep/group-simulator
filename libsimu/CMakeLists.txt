
set(LIBSIMU_SOURCES
  Costs.cpp
  Debug.cpp
  WCAEvent.cpp
  )

add_library(libsimu ${LIBSIMU_SOURCES})
llvm_config(libsimu Support)
