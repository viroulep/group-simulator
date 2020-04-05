cmake_minimum_required (VERSION 3.8)
project (GroupSimulator)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(BuildType)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -fno-exceptions)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Let's completely override CMake's default
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_subdirectory(libsimu)

# When running emcmake cmake, this variable is set to 1 :)

if(EMSCRIPTEN)
  message(STATUS "Emscripten detected, configuring to target Wasm.")
  add_subdirectory(wasm)
else()
  message(STATUS "Building regular programs")
  add_subdirectory(programs)
endif()
