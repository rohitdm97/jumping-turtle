#version 430 core
out vec4 FragColor;

uniform vec3 bold_color;

void main() {
	FragColor = vec4(bold_color, 1.0);
}
