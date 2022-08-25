#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 position = vec4(aPos.x, aPos.y - 5, aPos.z + 20, 1.0);
    vTexCoord = aTexCoord;

    gl_Position = projection * view * position;
}
