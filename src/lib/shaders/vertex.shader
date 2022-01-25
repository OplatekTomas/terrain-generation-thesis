#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position =  projection* view * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}