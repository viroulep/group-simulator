
set(SIMU_SOURCES
  simu.cpp
  )

set(OPT_SOURCES
  optimizer.cpp
  )

add_executable(group-simulator ${SIMU_SOURCES})
add_executable(optimizer ${OPT_SOURCES})

target_link_libraries(group-simulator PRIVATE libsimu)
target_link_libraries(optimizer PRIVATE libsimu)
