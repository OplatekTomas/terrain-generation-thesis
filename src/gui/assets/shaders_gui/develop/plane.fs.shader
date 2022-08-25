#version 430


in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2D tex;


void main() {
    fragColor = texture(tex, vTexCoord);
}