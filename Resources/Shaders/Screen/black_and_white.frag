#version 460 core
out vec4 FragColor;

in vec2 ouTexCoord;

uniform sampler2D unScreen;


void main() {
	FragColor = texture(unScreen, ouTexCoord);
	FragColor = vec4(vec3(FragColor.x * 0.2126 + FragColor.y * 0.7152 + FragColor.z * 0.0722), 1.0f);
}
