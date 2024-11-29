#version 430 core

in vec3 Position;

uniform mat4 Model;
uniform mat4 CameraView;
uniform mat4 CameraProjection;

out vec4 outPosition;

void main() {
	gl_Position = CameraProjection * CameraView * Model * vec4(Position, 1.0);
	outPosition = CameraView * Model * vec4(Position, 1.0);
}

