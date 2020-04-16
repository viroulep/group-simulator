set(SOURCES
  bindings.cpp
  )

# lol
set(CMAKE_EXECUTABLE_SUFFIX ".js")

set(DIST_DIR ${CMAKE_SOURCE_DIR}/dist)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG
  ${CMAKE_EXE_LINKER_FLAGS_DEBUG}
  "-s ASSERTIONS=1 -s SAFE_HEAP=1"
  )

# Interestingly, this is needed for the object and wasm to have matching signatures
# for all functions
add_compile_options(-flto)
add_link_options(
  --bind
  "SHELL:--closure 1"
  -flto
  "SHELL:--llvm-lto 1"
  "SHELL:-s EXPORT_ES6=1"
  "SHELL:-s MODULARIZE=1"
  "SHELL:-s ALLOW_MEMORY_GROWTH=1"
  )

add_executable(group-simulator ${SOURCES})
target_link_libraries(group-simulator libsimu)

# Copy the built file to the dist directory
add_custom_target(dist
  DEPENDS group-simulator)
add_custom_command(TARGET dist
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/group-simulator.js ${DIST_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/group-simulator.wasm ${DIST_DIR}/
  )
