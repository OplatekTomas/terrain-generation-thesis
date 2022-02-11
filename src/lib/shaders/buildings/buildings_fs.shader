#version 420 core
in vec4 gl_FragCoord;

in vec3 WorldPos_VS_Out;
in vec3 Normal_VS_Out;
out vec2 TexCoord_VS_Out;

out vec4 FragColor;
//in vec2 TexCoord;

uniform vec3 lightPos;


void main() {
    float ambientStrength = 0.1;
    vec3 lightColor = vec3(247, 245, 230) / 255.0;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal_VS_Out);
    vec3 lightDir = normalize(lightPos - WorldPos_VS_Out);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 baseColor = vec3(0.9);
    vec3 result = (ambient + diffuse) * baseColor;

    FragColor = vec4(result, 1.0);
}
