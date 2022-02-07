#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out vec3 WorldPos_CS_in;
out vec2 TexCoord_CS_in;
out vec3 Normal_CS_in;

uniform mat4 view;
uniform mat4 projection;

void main(){
	WorldPos_CS_in = aPos;
	TexCoord_CS_in = aTexCoord;
	Normal_CS_in = normalize(aNormal);
}