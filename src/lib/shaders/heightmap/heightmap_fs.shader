#version 420 core
layout(location = 0) out vec3 color;

in vec3 WorldPos_FS_in;
in vec2 TexCoord_FS_in;
in vec3 Normal_FS_in;

void main(){
    //color = vec3(WorldPos_FS_in.y);
    color = vec3(WorldPos_FS_in.y);

    //color = vec3(1);
}
