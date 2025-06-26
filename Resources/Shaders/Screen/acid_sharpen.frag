#version 460 core
out vec4 FragColor;

in vec2 ouTexCoord;

uniform sampler2D unScreen;

const float offset = 4.0f / 1280.0f;
const vec2 offsets[9] = vec2[](
	vec2(-offset, offset),
	vec2(0.0f, offset),
	vec2(offset, offset),
	vec2(-offset, 0.0f),
	vec2(0.0f, 0.0f),
	vec2(offset, 0.0f),
	vec2(-offset, -offset),
	vec2(0.0f, -offset),
	vec2(offset, -offset)
);
const float kernel[9] = float[](
	-1, -1, -1,
	-1,  9, -1,
	-1, -1, -1
);

void main() {
	FragColor = vec4(vec3(0.0f), 1.0f);
	for(int i = 0; i < 9; ++i) {
		FragColor += vec4(vec3(texture(unScreen, ouTexCoord.st+offsets[i])) * kernel[i], 1.0f);
	}
}
