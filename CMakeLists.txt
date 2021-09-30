cmake_minimum_required(VERSION 3.20.0)

PROJECT(vut-bp)

SET(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src/gui)
add_subdirectory(src/lib-run)
add_subdirectory(src/lib)
