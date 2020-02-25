
set(LIBSIMU_SOURCES
  CostsModel.cpp
  Debug.cpp
  )

add_library(libsimu ${LIBSIMU_SOURCES})
llvm_config(libsimu Support)
