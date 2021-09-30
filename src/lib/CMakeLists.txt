cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

set(CPM_DOWNLOAD_VERSION 0.32.0)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

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

#add packages
CPMAddPackage(gh:nlohmann/json@3.10.2)
CPMAddPackage(NAME cpr
        GIT_TAG 1.6.2
        GITHUB_REPOSITORY libcpr/cpr
        )

find_package(Boost 1.76.0 COMPONENTS filesystem)

add_library (lib
        CsvDecoder.h CsvDecoder.cpp
        api/ApiBase.h
        api/BingApi.cpp api/BingApi.h models/bing/ElevationResult.cpp models/bing/ElevationResult.h)


target_link_libraries(lib PRIVATE ${Boost_LIBRARIES})
target_link_libraries(lib PUBLIC cpr::cpr)
target_link_libraries(lib PUBLIC nlohmann_json::nlohmann_json)

target_include_directories (lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})




