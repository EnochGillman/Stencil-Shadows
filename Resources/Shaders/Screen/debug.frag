#version 460 core
out vec4 FragColor;

in vec2 ouTexCoord;

#define LIGHT_CALC 4.0f

uniform sampler2D unScreen;
uniform sampler2D unLighting;
uniform sampler2D unFog;
uniform sampler2D unDebug;
uniform vec3 unLightAc;

void main() {
	vec4 debug = texture(unDebug, ouTexCoord);
	FragColor = vec4(
		mix(
			mix(
				texture(unScreen, ouTexCoord).xyz
				* (unLightAc + texture(unLighting, ouTexCoord).xyz * LIGHT_CALC),
				unLightAc,
				texture(unFog, ouTexCoord).x
			),
			vec3(debug),
			debug.w
		)
		, 1.0f);
// 	if(gl_FragCoord.x < 800) {
// 		FragColor = texture(unDebug, ouTexCoord);
// 	}
	if(gl_FragCoord.x < 750) {
		FragColor = texture(unScreen, ouTexCoord);
	}
	if(gl_FragCoord.x < 500) {
		FragColor = texture(unLighting, ouTexCoord);
	}
	if(gl_FragCoord.x < 250) {
		FragColor = texture(unFog, ouTexCoord);
	}

}
