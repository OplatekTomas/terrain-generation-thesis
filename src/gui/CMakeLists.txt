cmake_minimum_required(VERSION 3.20)

project(gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPM_DOWNLOAD_VERSION 0.32.0)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

#set correct directory for CPM
if(CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()
#download cpm
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})
include(FetchContent)



set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp MainWindow.h MainWindow.ui opengl/Renderer.cpp opengl/Renderer.h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gui  MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    add_executable(gui  ${PROJECT_SOURCES})
endif()


target_link_libraries(gui  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets lib)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(vut-bp-gui)
endif()
