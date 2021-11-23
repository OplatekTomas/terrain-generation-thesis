#version 450
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture;

void main(){

    vec4 texColor = texture(Texture, TexCoord);

    FragColor = mix(vec4(0.2), texColor, texColor.a);

    //FragColor = color;
    //FragColor = texture(Texture, TexCoord);
}