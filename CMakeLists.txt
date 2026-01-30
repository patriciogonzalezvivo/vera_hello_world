cmake_minimum_required(VERSION 3.5)

project(vera_app LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-DGLM_FORCE_CXX11 -DGLM_FORCE_SWIZZLE -Wno-psabi)

# The compiled VERA 
add_subdirectory(deps)

file(GLOB ROOT_SOURCE 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(vera_app
    ${ROOT_SOURCE}
)

set_target_properties(vera_app PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# copy all files on assets on the build folder
file(COPY   assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

if (EMSCRIPTEN)
    # create a gsplat.html 
    set_target_properties(vera_app PROPERTIES SUFFIX ".html")

    # copy the index.html 
    file(COPY   assets/wasm/index.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    file(COPY   assets/wasm/main.js DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    file(COPY   assets/wasm/wasm-loader.js DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    file(COPY   assets/wasm/style.css DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    file(COPY   assets/wasm/thumbnail.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    file(COPY   assets/wasm/frame_background.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    file(COPY   assets/wasm/frame_refleccion.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

    set(LFLAGS "${LFLAGS} -s WASM=3")
    set(LFLAGS "${LFLAGS} -s USE_GLFW=3")
    set(LFLAGS "${LFLAGS} -s USE_WEBGL2=1")
    # set(LFLAGS "${LFLAGS} -s FULL_ES2=1")
    set(LFLAGS "${LFLAGS} -s FULL_ES3=1")
    # set(LFLAGS "${LFLAGS} -s MIN_WEBGL_VERSION=2")
    # set(LFLAGS "${LFLAGS} -s MAX_WEBGL_VERSION=2")
    # set(LFLAGS "${LFLAGS} -s WEBGL2_BACKWARDS_COMPATIBILITY_EMULATION=1")
    set(LFLAGS "${LFLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(LFLAGS "${LFLAGS} -s NO_DYNAMIC_EXECUTION=1")

    set(LFLAGS "${LFLAGS} --preload-file earth-water.png")
    set(LFLAGS "${LFLAGS} -s EXPORTED_FUNCTIONS='[  \"_main\", \"_malloc\" ]' ")
    set(LFLAGS "${LFLAGS} -s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]' ")
    
    set_target_properties(vera_app PROPERTIES LINK_FLAGS "${LFLAGS}")
    target_link_libraries(vera_app PRIVATE vera glfw webxr)
    
else()
    target_link_libraries(vera_app PRIVATE vera pthread dl)

endif()