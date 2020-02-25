
set(SIMU_SOURCES
  simu.cpp
  )

add_executable(group-simulator ${SIMU_SOURCES})

llvm_config(group-simulator Support)
#include_directories(${PROJECT_SOURCE_DIR}/v1/include ${PROJECT_SOURCE_DIR}/third_party/cxxopts ${YAML_CPP_INCLUDE_DIR})

target_link_libraries(group-simulator PRIVATE libsimu)
