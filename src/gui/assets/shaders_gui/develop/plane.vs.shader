#version 430 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 position = vec4(aPos.x, aPos.y - 5, aPos.z + 20, 1.0);

    gl_Position = projection * view * position;
}
