cmake_minimum_required(VERSION 3.20)

project(lib-run VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Boost 1.76 COMPONENTS format)

set(PROJECT_SOURCES
        main.cpp
        )


add_executable(lib-run ${PROJECT_SOURCES})
target_link_libraries(lib-run PUBLIC ${Boost_LIBRARIES})
target_link_libraries (lib-run LINK_PUBLIC lib)

