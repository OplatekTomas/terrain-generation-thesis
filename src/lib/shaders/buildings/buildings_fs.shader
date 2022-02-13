#version 420 core
in vec4 gl_FragCoord;

in vec3 WorldPos_VS_Out;
in vec3 Normal_VS_Out;
out vec2 TexCoord_VS_Out;

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
    vec3 color = CalcDirLight(Normal_VS_Out);
    FragColor = vec4(baseColor * color, 1.0);
}
