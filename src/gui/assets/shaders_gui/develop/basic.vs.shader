#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 inColor;

uniform mat4 view;
uniform mat4 projection;

out vec3 color;

void main() {
    vec4 position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    color = inColor;

    gl_Position = projection * view * position;
}
