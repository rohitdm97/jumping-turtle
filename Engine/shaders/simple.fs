#version 430 core
out vec4 FragColor;

uniform vec4 ecolor;

void main() {
	vec4 a = ecolor;
	FragColor = vec4(ecolor.xyz, 1.0);
}

