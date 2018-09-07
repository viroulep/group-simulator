# Switch to -O when done, fix the things so that -fno-rtti is possible
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

add_executable(simu minisim.cpp costs.cpp generator.cpp judge_runs_system.cpp runner_system.cpp simu.cpp types.cpp)

find_package(yaml-cpp REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/third_party/cxxopts ${YAML_CPP_INCLUDE_DIR})

target_link_libraries(simu ${YAML_CPP_LIBRARIES})
