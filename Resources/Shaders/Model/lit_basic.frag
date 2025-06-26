#version 460 core
layout (location = 2) out vec4 FragColor;

in DATA {
	vec3 Pos;
	vec3 Normal;
	vec2 TexCoord;
} ou;

#define LIGHT_CALC 0.25f

uniform vec3 unLightAc;
uniform vec3 unViewPos;

float length2(vec3 vector) {
	return dot(vector, vector);
}

float lightDist(float units, float dist) {
	return pow(1.0f - dist / units, 6);
}

struct Mat {
	sampler2D diff1;
	sampler2D spec1;
	float str;
};

uniform Mat unMat;
uniform sampler2D unLighting;

uniform int unWidth;
uniform int unHeight;

uniform struct Light {
	vec4 pos;
	vec3 dir;
	vec3 col;
	float dist;
	float cutOff;
	float cutSmo;
} unLight;

void main() {
	vec3 lighting = vec3(0.0f);
	if(0 < unLight.cutOff) {		//Flashlight
		if(length2(ou.Pos - vec3(unLight.pos)) < unLight.dist * unLight.dist) {
			vec3 viewDir = normalize(vec3(unLight.pos) - ou.Pos);
			float theta = dot(viewDir, normalize(unLight.dir));
			if(theta < -unLight.cutOff) {
				lighting +=
					unLight.col
					* (
						//diff1use
						max(dot(ou.Normal, viewDir), 0.0f)
						//spec1ular
						+ pow(max(dot(ou.Normal, normalize(
							viewDir
							+ normalize(unViewPos - ou.Pos)
						)), 0.0f), 16)
						* texture(unMat.spec1, ou.TexCoord).y
					)
					* lightDist(unLight.dist, length(ou.Pos - vec3(unLight.pos)))
					* min(1.0f, (theta + unLight.cutOff) / (unLight.cutSmo - unLight.cutOff));
			}
		} else {
			discard;
		}
	} else if(unLight.pos.w == 0.0f) {	//Directional Light
		vec3 viewDir = normalize(vec3(unLight.pos));
		lighting +=
			unLight.col
			* (
				//diff1use
				max(dot(ou.Normal, viewDir), 0.0f)
				//spec1ular
				+ pow(max(dot(ou.Normal, normalize(
					viewDir
					+ normalize(unViewPos - ou.Pos)
				)), 0.0f), 16)
				* texture(unMat.spec1, ou.TexCoord).y
			);
	} else {	//Point Light
		if(length2(ou.Pos - vec3(unLight.pos)) < unLight.dist * unLight.dist) {
			vec3 viewDir = normalize(vec3(unLight.pos) - ou.Pos);
			lighting +=
				unLight.col
				* (
					//diff1use
					max(dot(ou.Normal, viewDir), 0.0f)
					//spec1ular
					+ pow(max(dot(ou.Normal, normalize(
						viewDir
						+ normalize(unViewPos - ou.Pos)
					)), 0.0f), 16)
					* texture(unMat.spec1, ou.TexCoord).y
				)
				* lightDist(unLight.dist, length(ou.Pos - vec3(unLight.pos)));
		} else {
			discard;
		}
	}
	FragColor = vec4(lighting * LIGHT_CALC, 1.0f);
}
