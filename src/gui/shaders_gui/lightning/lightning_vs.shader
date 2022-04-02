#version 420 core


layout(location = 0) in vec3 position;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(position, 1.0);
    TexCoords = vec2(position.x + 1, position.y + 1) / 2.0;
}
