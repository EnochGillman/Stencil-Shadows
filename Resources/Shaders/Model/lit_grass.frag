#version 460 core
out vec4 FragColor;

in vec3 ouPos;
in vec3 ouNormal;
in vec2 ouTexCoord;
in float ouFog;

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

struct Light {
	vec4 pos;
	vec3 dir;
	vec3 col;
	float dist;
	float cutOff;
	float cutSmo;
};

#define PI float(3.14159265358979323846)
#define NUM_LIGHTS 8
uniform int unLightSize;
uniform Light unLights[NUM_LIGHTS];

void main() {
	FragColor = texture(unMat.diff1, ouTexCoord);
	if(FragColor.w < 0.5f) {
		discard;
	}
	vec3 lighting = unLightAc;
	for(int i = 0; i < unLightSize; ++i) {
		if(0 < unLights[i].cutOff) {		//Flashlight
			if(length2(ouPos - vec3(unLights[i].pos)) < unLights[i].dist * unLights[i].dist) {
				vec3 viewDir = normalize(vec3(unLights[i].pos) - ouPos);
				float theta = dot(viewDir, normalize(unLights[i].dir));
				if(theta < -unLights[i].cutOff) {
					lighting +=
						unLights[i].col
						* (
							//diff1use
							max(dot(ouNormal, viewDir), 0.0f)
						)
						* lightDist(unLights[i].dist, length(ouPos - vec3(unLights[i].pos)))
						* min(1.0f, (theta + unLights[i].cutOff) / (unLights[i].cutSmo - unLights[i].cutOff));
				}
			}
		} else if(unLights[i].pos.w == 0.0f) {	//Directional Light
			vec3 viewDir = normalize(unLights[i].dir);
			lighting +=
				unLights[i].col;
		} else {	//Point Light
			if(length2(ouPos - vec3(unLights[i].pos)) < unLights[i].dist * unLights[i].dist) {
				vec3 viewDir = normalize(vec3(unLights[i].pos) - ouPos);
				lighting +=
					unLights[i].col
					* lightDist(unLights[i].dist, length(ouPos - vec3(unLights[i].pos)));
			}
		}
	}
	FragColor = mix(
// 		vec4(0.5f, 0.5f, 0.5f, 1.0f) * vec4(lighting, 1.0f),
		FragColor * vec4(lighting, 1.0f),
		vec4(unLightAc, 1.0f),
		ouFog
	);
}
