#version 460 core
layout (location = 3) out vec4 FragColor;

in vec3 ouColor;

void main() {
	FragColor = vec4(ouColor, 1.0f);
}
