#version 420 core

out vec4 FragColor;

in vec3 TexCoords;

layout(binding=0)uniform samplerCube skybox;

void main()
{
    vec3 color = texture(skybox, TexCoords).rgb;
    FragColor = vec4(color, 1.0);
}