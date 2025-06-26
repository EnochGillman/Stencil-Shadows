#version 460 core
out vec4 FragColor;

in vec2 ouTexCoord;

uniform sampler2D unScreen;


void main() {
	FragColor = texture(unScreen, ouTexCoord);
	float sat = 1;//(sin(gl_FragCoord.x/10)+sin(gl_FragCoord.x/10+gl_FragCoord.y/10))*1.5f;
	FragColor = vec4(vec3(FragColor) * vec3(1 + sat) - vec3(FragColor.x * 0.2126 + FragColor.y * 0.7152 + FragColor.z * 0.0722) * sat, FragColor.w);
}
