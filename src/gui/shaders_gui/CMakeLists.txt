
#Used to transfer shaders files to custom files that can be included in C++ code by Shaders.h

file(GLOB_RECURSE SHADERS_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.shader)
foreach(file ${SHADERS_SRC_FILES})
    #Get file name after shaders dir
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/shaders_gui/" "" PATH_IN_SHADERS_DIR "${file}")
    #Fill the file with shader data
    set(DUMMY_FILE "${CMAKE_CURRENT_BINARY_DIR}/shaders_gui/include/${PATH_IN_SHADERS_DIR}.out")
    message(STATUS "Generating ${DUMMY_FILE}")
    file(READ "${file}" DUMMY_CONTENT)
    file(WRITE "${DUMMY_FILE}" "R\"=====(${DUMMY_CONTENT})=====\"")
endforeach()