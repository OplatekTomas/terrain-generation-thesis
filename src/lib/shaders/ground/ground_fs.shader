#version 420 core

in vec3 WorldPos_FS_in;
in vec2 TexCoord_FS_in;
in vec3 Normal_FS_in;

uniform vec3 lightPos;
layout (binding = 0) uniform sampler2D MetadataTexture;
layout (binding = 1) uniform sampler2DArray AsphaltTextures;
layout (binding = 2) uniform sampler2DArray FieldTextures;
layout (binding = 3) uniform sampler2DArray GrassTextures;
layout (binding = 4) uniform sampler2DArray ForestTexture;
layout (binding = 5) uniform sampler2DArray WaterTexture;
layout (binding = 6) uniform sampler2DArray FootPathTexture;
layout (binding = 7) uniform sampler2DArray PathTexture;
layout (binding = 8) uniform sampler2DArray SandTexture;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gSpecular;


bool equals(float a, float b) {
    b = b / 255.0;
    return abs(a - b) < 0.000001;
}

vec4 getColor(float biome, vec2 texCoord, float scale, sampler2DArray tex){
    texCoord = mod(texCoord * scale, 1.0);
    vec4 color = vec4(0.0);
    color = vec4(texture(tex, vec3(texCoord, 0.0)).rgb, 1.0);
    return color;
}

void main(){
    vec4 metadata = texture(MetadataTexture, TexCoord_FS_in);
    float r = metadata.r;
    vec4 color;
    vec4 specular = vec4(0.0);
    if (r == 255){ // Pixel is used as a "unkown" area
        color = vec4(0.5, 0.5, 0.5, 1.0);
    } else if (equals(r, 1.0) || equals(r, 8.0) || equals(r, 9.0)){ //Pixel is used as a forrest
        color = getColor(r, TexCoord_FS_in, 100, ForestTexture);
    } else if (equals(r, 2.0)){ //Pixel is used as a water
        color = getColor(r, TexCoord_FS_in, 75, WaterTexture) * 0.5;
        specular = vec4(0.0, 0.0, 0.5, 1.0);
    } else if (equals(r, 3.0)){ //Pixel is used as a field
        color =  getColor(r, TexCoord_FS_in, 100, FieldTextures);
    } else if (equals(r, 4.0)){ //Pixel is used as a road
        color =  getColor(r, TexCoord_FS_in, 100, AsphaltTextures);
    } else if (equals(r, 5.0)){ //Pixel is used as a building
        color = vec4(0.78, 0.78, 0.78, 1.0);
    } else if (equals(r, 6.0)){ //Pixel is used as a park
        color = getColor(r, TexCoord_FS_in, 150, GrassTextures);
    } else if (equals(r, 7.0)){ //Pixel is used as a residential/industrial area
        color = vec4(0.5, 0.5, 0.5, 1.0);
    } else if (equals(r, 10.0)){ // paved ish path
        color = getColor(r, TexCoord_FS_in, 120, FootPathTexture);
    } else if (equals(r, 11.0)){ // Normal path
        color = vec4(0.4, 0.23, 0, 1.0);
    }else if(equals(r, 12.0)){ //sandy stuff
        color = getColor(r, TexCoord_FS_in, 150, SandTexture);
    }

    else {
        discard; //We have no idea - discard
    }

    gPosition = WorldPos_FS_in.xyz;
    gNormal = normalize(Normal_FS_in).xyz;
    gAlbedo = vec4(color.rgb, 1.0);
    gSpecular = specular;
}
