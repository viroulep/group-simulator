set(SOURCES
  hello.cpp
  )
#https://stackoverflow.com/questions/15865923/interaction-with-c-classes-in-emscripten
#https://github.com/webpack/webpack/issues/7352
#https://koala42.com/using-webassembly-in-your-reactjs-app/
#https://github.com/emscripten-core/emscripten/wiki/WebAssembly-Standalone
#https://prestonrichey.com/blog/react-rust-wasm/

#https://github.com/dkirkman/ws-emscripten-webpack/tree/master/emscripten-webpack

#https://github.com/esa/dcgp-web/
# without webpacker...
#https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html

# hope: ?
#https://stackoverflow.com/questions/54115783/how-to-use-emscripten-compiled-javascript-within-react-react-native
#https://github.com/cggallant/VuejsGlobalInstance/tree/master/VuejsGlobalInstance
# hope!
#https://developers.google.com/web/updates/2019/01/emscripten-npm
#https://gist.github.com/surma/9899231095ada390b2b178a72ff57aa3
# disabled eslint!
# changed env
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/premod.js ${CMAKE_CURRENT_BINARY_DIR}/premod.js COPYONLY)

# lol
set(CMAKE_EXECUTABLE_SUFFIX ".js")
add_compile_options(-flto)
add_link_options(
  #--pre-js premod.js
  --bind
  "SHELL:--closure 1"
  -flto
  "SHELL:--llvm-lto 1"
  #"SHELL:-s ENVIRONMENT='web'"
  "SHELL:-s EXPORT_ES6=1"
  "SHELL:-s MODULARIZE=1"
  )

add_executable(hello ${SOURCES})
target_link_libraries(hello libsimu)
