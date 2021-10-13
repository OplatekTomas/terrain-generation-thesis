cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPM_DOWNLOAD_VERSION 0.32.0)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
include(FetchContent)


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
CPMAddPackage(
        NAME nlohmann_json
        VERSION 3.10.2
        # the git repo is incredibly large, so we download the archived include directory
        URL https://github.com/nlohmann/json/releases/download/v3.10.2/include.zip
)

CPMAddPackage(
        NAME cpr
        GITHUB_REPOSITORY libcpr/cpr
        GIT_TAG 290fbaff7321b3df66c4ac5b039815f25e4333bb
)

CPMAddPackage("gh:fmtlib/fmt#8.0.1")



add_library (lib
        config/Config.h
        api/ApiBase.h api/BingApi.cpp api/BingApi.h
        models/bing/ElevationResult.h api/ApiBase.cpp api/ApiBase_impl.h config/ConfigReader.cpp config/ConfigReader.h JsonHelper.h MapGenerator.cpp MapGenerator.h VertexData.cpp VertexData.h)


if (nlohmann_json_ADDED)
    target_include_directories(lib PUBLIC ${nlohmann_json_SOURCE_DIR}/include)
endif()
target_link_libraries(lib PUBLIC cpr::cpr fmt)

target_include_directories (lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})




