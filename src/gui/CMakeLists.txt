cmake_minimum_required(VERSION 3.20)

project(gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPM_DOWNLOAD_VERSION 0.32.0)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")


find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
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

CPMAddPackage(
        NAME GPUEngine
        OPTIONS "GPUENGINE_BUILD_GESG ON"
        GITHUB_REPOSITORY Rendering-FIT/GPUEngine
        GIT_TAG 53c587c52d99b57e77817bb4df1d625fcea1d5be
)



find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp MainWindow.h MainWindow.ui
        renderer/Camera.cpp renderer/Camera.h renderer/Scene3D.cpp renderer/Scene3D.h shaders_gui/Shaders.h StaticFunctions.h renderer/SSAO.cpp renderer/SSAO.h shaders_gui/Shaders.cpp StaticFunctions.cpp renderer/Skybox.cpp renderer/Skybox.h renderer/Renderer.cpp renderer/Renderer.h ui/Map.cpp ui/Map.h ui/mapview.cpp ui/mapview.h ui/mapview.ui ui/rendererlayout.cpp ui/rendererlayout.h ui/rendererlayout.ui)

OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3 ")
    ENDIF()
ENDIF()

#Create dummy files so that they at least exist when CMake project is loaded
file(GLOB_RECURSE SHADERS_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders_gui/*.shader)
set(SHADERS_DST_FILES "")
foreach(file ${SHADERS_SRC_FILES})
    #Get file name after shaders dir
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/shaders_gui/" "" PATH_IN_SHADERS_DIR "${file}")
    #Create a dummy file
    set(DUMMY_FILE "${CMAKE_CURRENT_BINARY_DIR}/shaders_gui/include/${PATH_IN_SHADERS_DIR}.out")
    message(STATUS "Creating dummy shader file ${DUMMY_FILE}")
    file(WRITE "${DUMMY_FILE}" "")
    set(SHADERS_DST_FILES "${SHADERS_DST_FILES};${DUMMY_FILE}")
endforeach()
message(STATUS "Found ${SHADERS_SRC_FILES} shader files")

message(STATUS "Created dummy shader files ${SHADERS_DST_FILES}")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}  MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${SHADER_DST_FILES})
endif()




find_package(GPUEngine COMPONENTS geGL geSG geUtil)
find_package(Marble REQUIRED)

set(SHADER_COPY_GUI shaderCopyTargetGUI)
add_custom_target(shaderCopyTargetGUI
        COMMAND ${CMAKE_COMMAND} -P "shaders_gui/CMakeLists.txt"
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        )

set(SHADER_COPY_GUI_HELP shaderCopyTargetGUIHelp)
add_custom_target(shaderCopyTargetGUIHelp
        COMMAND ${CMAKE_COMMAND} -E copy_directory shaders_gui/include ${CMAKE_CURRENT_BINARY_DIR}/shaders_gui/include
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        )
set(TEXTURE_COPY_GUI textureCopyTarget)

add_custom_target(textureCopyTarget
        COMMAND ${CMAKE_COMMAND} -E copy_directory renderer/textures ${CMAKE_CURRENT_BINARY_DIR}/renderer/textures
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_dependencies(${SHADER_COPY_GUI_HELP} ${SHADER_COPY_GUI})
add_dependencies(${PROJECT_NAME} ${SHADER_COPY_GUI_HELP} ${TEXTURE_COPY_GUI})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(vut-bp-gui)
endif()
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/shaders_gui/include/lighting)
target_link_libraries(${PROJECT_NAME} PRIVATE Marble Qt${QT_VERSION_MAJOR}::Widgets geGL geSG geUtil lib ${PNG_LIBRARY})