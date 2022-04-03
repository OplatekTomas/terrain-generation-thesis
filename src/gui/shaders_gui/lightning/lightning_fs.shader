#version 420 core


out vec4 FragColor;
in vec2 TexCoords;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;
layout(binding = 3) uniform sampler2D ssao;

vec3 CalcDirLight(vec3 normal){
    //Get AO
    float ao = texture(ssao, TexCoords).r;

    //Calculate ambient strengh
    float ambientStrength = 0.25;
    vec3 ambientColor = vec3(135, 206, 235) / 255.0;
    vec3 ambient = ambientStrength * ambientColor * ao;

    //calculate diffuse strength
    vec3 diffuseColor = vec3(256,251,211) / 255.0;
    vec3 lightDir = vec3(0.5, 1.0, 0.0);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;
    return (ambient + diffuse);
}

void main() {
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 light = CalcDirLight(Normal);
    vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
    float ao = texture(ssao, TexCoords).r;
    //FragColor = vec4(Albedo * light, 1.0);
    FragColor = vec4(ao, ao, ao ,1.0);
}