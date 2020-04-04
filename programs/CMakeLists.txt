# Build the actual library
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM Root: ${LLVM_TOOLS_BINARY_DIR}")

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

# LLVM 7 headers have an annoying class-memaccess warning
add_compile_options(-Wno-class-memaccess)

include_directories(${LLVM_INCLUDE_DIRS})

set(SIMU_SOURCES
  simu.cpp
  config_loader.cpp
  )

set(OPT_SOURCES
  optimizer.cpp
  config_loader.cpp
  )

add_executable(group-simulator ${SIMU_SOURCES})
add_executable(optimizer ${OPT_SOURCES})

llvm_config(group-simulator Support)
llvm_config(optimizer Support)


target_link_libraries(group-simulator PRIVATE libsimu)
target_link_libraries(optimizer PRIVATE libsimu)
