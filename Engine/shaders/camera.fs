#version 430 core
out vec4 FragColor;

in vec4 vPosition;
in vec2 vTexCoord;

uniform sampler2D diffuse;

void main() {
	FragColor = mix(texture(diffuse, vTexCoord), vec4(vTexCoord.x, 0.0, vTexCoord.y, 1.0), 0.001);
}

