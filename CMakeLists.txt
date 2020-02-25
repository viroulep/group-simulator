cmake_minimum_required (VERSION 3.6)
project (GroupSimulator)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM Root: ${LLVM_TOOLS_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)


list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

#add_subdirectory(v1)
include_directories(${LLVM_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
add_compile_options("-fno-rtti" "-Wall")

if(NOT CMAKE_BUILD_TYPE STREQUAL release)
  add_compile_options("-g")
endif()

add_subdirectory(libsimu)
add_subdirectory(src)
