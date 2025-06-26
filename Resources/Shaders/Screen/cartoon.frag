#version 460 core
out vec4 FragColor;

in vec2 ouTexCoord;

#define LIGHT_CALC 4.0f

uniform sampler2D unScreen;
uniform sampler2D unLighting;
uniform sampler2D unFog;
uniform sampler2D unDebug;
uniform vec3 unLightAc;

const float os = 2.0f / 1280.0f;
const vec2 offsets[9] = vec2[](
	vec2(-os*0.707, os*0.707), vec2(0.0f, os), vec2(os*0.707, os*0.707),
	vec2(-os, 0.0f), vec2(0.0f, 0.0f), vec2(os, 0.0f),
	vec2(-os*0.707, -os*0.707), vec2(0.0f, -os), vec2(os*0.707, -os*0.707)
);
const float kernel[9] = float[](
	1,  2,   1,
	2,  -12, 2,
	1,  2,   1
);

void main() {
	float OutlineColor = 1.0f;
	float fog = texture(unFog, ouTexCoord).x;
	for(int i = 0; i < 9; ++i) {
		if(fog + 0.03 < texture(unFog, ouTexCoord.st+offsets[i]).x && i != 4) {
			OutlineColor = 0.0f;
			break;
		}
	}

	vec4 debug = texture(unDebug, ouTexCoord);
	FragColor = vec4(
		mix(
			mix(
				texture(unScreen, ouTexCoord).xyz
				* (unLightAc + texture(unLighting, ouTexCoord).xyz * LIGHT_CALC)
				* OutlineColor,
				unLightAc,
				fog
			),
			vec3(debug),
			debug.w
		)
		, 1.0f);

	//Change this to increase color levels
	int comp = 8;

	float bright = round((FragColor.x * 0.2126 + FragColor.y * 0.7152 + FragColor.z * 0.0722) * comp + 0.25) / comp;

	float red = (FragColor.x + bright) / 2;
	float green = (FragColor.y + bright) / 2;
	float blue = (FragColor.z + bright) / 2;

	float gray = red * 0.2126 + green * 0.7152 + blue * 0.0722;
	float sat = 2;

	red = red * (1 + sat) - gray * sat;
	green = green * (1 + sat) - gray * sat;
	blue = blue * (1 + sat) - gray * sat;

	FragColor = vec4(red, green, blue, 1.0f);
}
