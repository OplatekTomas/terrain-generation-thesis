#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec2 instances;

out vec3 WorldPos_VS_Out;
out vec2 TexCoord_VS_Out;
out vec3 Normal_VS_Out;


uniform mat4 view;
uniform mat4 projection;
uniform sampler2D heightMap;
uniform float scaleZ;
uniform float scaleX;


void main() {
    vec2 shift = instances;
    vec3 pos = vec3(position.x + shift.x, position.y, position.z + shift.y);
    pos = vec3((pos.x * scaleX) + ((1-scaleX) / 2), pos.y, (pos.z * scaleZ) + ((1-scaleZ) / 2));

    vec2 texPos = vec2(1-pos.x, pos.z);
    float height = texture(heightMap, texPos).r;
    if (height != 0.0){
        pos.y += height;
    }

    TexCoord_VS_Out = texCoord;
    Normal_VS_Out = normalize(normal);
    gl_Position = projection * view * vec4(pos, 1.0);
    WorldPos_VS_Out = (view * vec4(pos, 1.0)).xyz;
}
