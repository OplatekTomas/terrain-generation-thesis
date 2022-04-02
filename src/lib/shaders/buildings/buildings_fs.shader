#version 420 core
in vec4 gl_FragCoord;

in vec3 WorldPos_VS_Out;
in vec3 Normal_VS_Out;
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;


out vec4 FragColor;
//in vec2 TexCoord;

uniform vec3 lightPos;

vec3 CalcDirLight(vec3 normal){
    //Calculate ambient strengh
    float ambientStrength = 0.25;
    vec3 ambientColor = vec3(135, 206, 235) / 255.0;
    vec3 ambient = ambientStrength * ambientColor;
    //calculate diffuse strength
    vec3 diffuseColor = vec3(256,251,211) / 255.0;
    vec3 lightDir = vec3(0.5, 1.0, 0.0);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;
    return (ambient + diffuse);
}


void main() {
    vec3 baseColor = vec3(70, 70, 100) / 255.0;
    //vec3 color = CalcDirLight(Normal_VS_Out);
    gPosition = WorldPos_VS_Out;
    gNormal = normalize(Normal_VS_Out);
    gAlbedo.rgb = baseColor.rgb;
}
