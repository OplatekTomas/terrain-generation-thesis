#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out vec3 WorldPos_VS_Out;
out vec2 TexCoord_VS_Out;
out vec3 Normal_VS_Out;

uniform mat4 view;
uniform mat4 projection;

void main(){
	WorldPos_VS_Out = aPos;
	TexCoord_VS_Out = aTexCoord;
	Normal_VS_Out = normalize(aNormal);
}