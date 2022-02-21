#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 WorldPos_VS_Out;
out vec2 TexCoord_VS_Out;
out vec3 Normal_VS_Out;


uniform mat4 view;
uniform mat4 projection;
uniform sampler2D heightMap;

void main() {
    vec3 pos = position;
    vec2 texPos = vec2(1-pos.x, pos.z);
    pos.y += texture(heightMap, texPos).r;

    WorldPos_VS_Out = pos;
    TexCoord_VS_Out = texCoord;
    Normal_VS_Out = normalize(normal);
    gl_Position = projection * view * vec4(pos, 1.0);
    //gl_Position = vec4(pos, 1.0);
}
