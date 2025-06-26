#version 460 core
out vec4 FragColor;

in vec2 ouTexCoord;

uniform sampler2D unScreen;

const float os = 2.0f / 1280.0f;
const vec2 offsets[9] = vec2[](
	vec2(-os, os), vec2(0.0f, os), vec2(os, os),
	vec2(-os, 0.0f), vec2(0.0f, 0.0f), vec2(os, 0.0f),
	vec2(-os, -os), vec2(0.0f, -os), vec2(os, -os)
);
const float kernel[9] = float[](
	 1,  2,   1,
	 2,  -12, 2,
	 1,  2,   1
);

void main() {
	FragColor = vec4(vec3(0.0f), 1.0f);
	for(int i = 0; i < 9; ++i) {
		FragColor += vec4(vec3(texture(unScreen, ouTexCoord.st+offsets[i])) * kernel[i], 1.0f);
	}
	FragColor = vec4(texture(unScreen, ouTexCoord)) - vec4(vec3(max(0.0f, FragColor.x * 0.2126 + FragColor.y * 0.7152 + FragColor.z * 0.0722)), 0.0f);
}
