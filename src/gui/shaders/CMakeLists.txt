
#Used to transfer shaders files to custom files that can be included in C++ code by Shaders.h

file(READ shaders/fragment.shader FRAG_SHADER)
file(WRITE shaders/include/shader.fs "R\"=====(${FRAG_SHADER})=====\"")

file(READ shaders/vertex.shader VERTEX_SHADER)
file(WRITE shaders/include/shader.vs "R\"=====(${VERTEX_SHADER})=====\"")