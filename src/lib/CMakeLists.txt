cmake_minimum_required(VERSION 3.20)

project("lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPM_DOWNLOAD_VERSION 0.32.0)
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
CPMAddPackage(
        NAME poly2tri
        GITHUB_REPOSITORY jhasse/poly2tri
        GIT_TAG 8b5fa15800ab8ef44ed7b0ec7061e907d5e49059
)


CPMAddPackage("gh:fmtlib/fmt#8.0.1")

CPMAddPackage(
        NAME Ccache.cmake
        GITHUB_REPOSITORY TheLartians/Ccache.cmake
        VERSION 1.2
)

FetchContent_Declare(
        boolinq
        URL https://raw.githubusercontent.com/k06a/boolinq/master/include/boolinq/boolinq.h
        DOWNLOAD_NO_EXTRACT TRUE
)

FetchContent_MakeAvailable(boolinq)

OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
    ENDIF()
ENDIF()


#Create dummy files so that they at least exist when CMake project is loaded
file(GLOB_RECURSE SHADERS_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.shader)
set(SHADERS_DST_FILES "")
foreach(file ${SHADERS_SRC_FILES})
    #Get file name after shaders dir
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/shaders/" "" PATH_IN_SHADERS_DIR "${file}")
    #Create a dummy file
    set(DUMMY_FILE "${CMAKE_CURRENT_BINARY_DIR}/shaders/include/${PATH_IN_SHADERS_DIR}.out")
    message(STATUS "Creating dummy shader file ${DUMMY_FILE}")
    file(WRITE "${DUMMY_FILE}" "")
    set(SHADERS_DST_FILES "${SHADERS_DST_FILES};${DUMMY_FILE}" scene/Program.cpp scene/Program.h generators/surface/SurfaceGenerator.cpp generators/surface/SurfaceGenerator.h generators/buildings/BuildingsGenerator.cpp generators/buildings/BuildingsGenerator.h generators/vegetation/PositionSampler.cpp generators/vegetation/PositionSampler.h)
endforeach()
message(STATUS "Found ${SHADERS_SRC_FILES} shader files")

add_library (lib
        shaders/Shaders.h ${SHADERS_DST_FILES}
        config/LibConfig.h
        api/ApiBase.h api/BingApi.cpp api/BingApi.h
        api/models/bing/ElevationResult.h api/ApiBase.cpp api/ApiBase_impl.h config/ConfigReader.cpp
        config/ConfigReader.h JsonHelper.h MapGenerator.cpp MapGenerator.h
        api/OpenStreetMapApi.cpp api/OpenStreetMapApi.h api/models/bing/ElevationData.cpp api/models/bing/ElevationData.h api/models/openstreetmap/OSMData.cpp api/models/openstreetmap/OSMData.h
        api/models/openstreetmap/MetadataResult.h Helper.h
        generators/landtype/LandTypeGenerator.cpp generators/landtype/LandTypeGenerator.h generators/landtype/AreaOnMap.cpp generators/landtype/AreaOnMap.h
        scene/Scene.cpp scene/Scene.h scene/Texture.cpp scene/Texture.h scene/Model.cpp scene/Model.h config/GeneratorOptions.h scene/Shader.cpp scene/Shader.h
        scene/Vertex.h scene/Vertex.cpp
        )

if (nlohmann_json_ADDED)
    target_include_directories(lib PUBLIC ${nlohmann_json_SOURCE_DIR}/include)
endif()

#Create fake target that calls the shaders/CMakeLists.txt file just before compile time to include the current version
#of a shader and not the one that would be read during loading of the project
set(FAKE_TARGET fakeTarget)
add_custom_target(fakeTarget
        COMMAND ${CMAKE_COMMAND} -P "shaders/CMakeLists.txt"
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        )
add_dependencies(${PROJECT_NAME} ${FAKE_TARGET})

target_link_libraries(lib PUBLIC cpr::cpr fmt poly2tri)

target_include_directories (lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(lib PUBLIC ${boolinq_SOURCE_DIR})




