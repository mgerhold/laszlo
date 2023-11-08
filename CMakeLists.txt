cmake_minimum_required(VERSION 3.27)
project(laszlo)

set(CMAKE_CXX_STANDARD 23)

if (EMSCRIPTEN)
    message("Emscripten build")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexperimental-library")
endif()

add_subdirectory(src)
