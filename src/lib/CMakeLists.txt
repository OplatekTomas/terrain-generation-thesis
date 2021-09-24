cmake_minimum_required(VERSION 3.20)

SET(CMAKE_CXX_STANDARD 20)

add_library (lib
        test.h
        test.cpp
        )

target_include_directories (lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})




