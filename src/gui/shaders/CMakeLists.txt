
#Used to transfer glsl files to custom files that can be included in C++ code by Shaders.h

file(READ shaders/fragment.glsl FRAG_SHADER)
file(WRITE shaders/include/shader.vs "R\"=====(${FRAG_SHADER})=====\"")

file(READ shaders/vertex.glsl VERTEX_SHADER)
file(WRITE shaders/include/shader.fs "R\"=====(${VERTEX_SHADER})=====\"")