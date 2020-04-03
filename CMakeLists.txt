cmake_minimum_required (VERSION 3.6)
project (GroupSimulator)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM Root: ${LLVM_TOOLS_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(BuildType)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
# LLVM 7 headers have an annoying class-memaccess warning
add_compile_options(-fno-rtti -Wall -Wno-class-memaccess -fno-exceptions)

# Let's completely override CMake's default
set(CMAKE_CXX_FLAGS_RELEASE "-s -O3 -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,--gc-sections")

add_subdirectory(libsimu)
add_subdirectory(src)
