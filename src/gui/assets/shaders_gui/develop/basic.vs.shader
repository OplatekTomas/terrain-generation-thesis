#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 inColor;

uniform mat4 view;
uniform mat4 projection;
uniform vec2 position;

void main() {
    vec2 offest = position;
    vec4 pos = vec4(aPos.x + offest.x - 7, aPos.y - 3.5, aPos.z + offest.y + 13, 1.0);
    gl_Position = projection * view * pos;
}
