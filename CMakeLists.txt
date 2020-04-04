cmake_minimum_required (VERSION 3.6)
project (GroupSimulator)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(BuildType)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(COMPILE_WASM "Compile the WASM wrapper (if enabled, won't compile the library but just the WASM wrapper." [OFF])

add_compile_options(-fno-rtti -Wall -Wno-class-memaccess -fno-exceptions)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Let's completely override CMake's default
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,--gc-sections")

add_subdirectory(libsimu)
if(COMPILE_WASM)
  # TODO: check called with emcmake!
  # TODO: find(libsimu.a)
  # TODO: find(em++)
  add_subdirectory(wasm)
else()
  add_subdirectory(src)
endif()
