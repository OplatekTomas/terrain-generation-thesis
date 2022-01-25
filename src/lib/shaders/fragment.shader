#version 450
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture;

bool equals(float a, float b) {
	return abs(a - b) < 0.01;
}

void main(){

	vec4 metadata = texture(Texture, TexCoord);
	float r = metadata.r;
	vec4 color;
	if (equals(r, 1.0f)){ // Pixel is used as a "unkown" area
		color = vec4(1.0);
	} else if (equals(r, 0.1)){ //Pixel is used as a forrest
		color = vec4(0, 0.29, 0.04, 1);
	} else if (equals(r, 0.2)){ //Pixel is used as a water
		color = vec4(0.0, 0.0, 0.5, 1.0);
	} else if (equals(r, 0.3)){ //Pixel is used as a field
		color = vec4(0.5, 0.5, 0.0, 1.0);
	} else if (equals(r, 0.4)){ //Pixel is used as a road
		color = vec4(0.7, 0.32, 0.08, 1.0);
	} else if (equals(r, 0.5)){ //Pixel is used as a building
		color = vec4(0.78, 0.78, 0.78, 1.0);
	} else if (equals(r, 0.6)){ //Pixel is used as a park
		color = vec4(0.0, 0.5, 0.0, 1.0);
	} else if (equals(r, 0.7)){ //Pixel is used as a residential/industrial area
		color = vec4(0.5, 0.5, 0.5, 1.0);
	} else {
		color = vec4(r, r, r, 1.0f);
	}
	FragColor = vec4(1.0);

	//FragColor = color;
	//FragColor = texture(Texture, TexCoord);
}