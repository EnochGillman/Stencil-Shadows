#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in vec4 ouPos[1];

out vec3 ouColor;

uniform vec3 unColor = vec3(1.0f);
uniform vec3 unViewPos;
uniform mat4 unView;
uniform mat3 unBillboard;


void main() {
	ouColor = unColor;
	for(int i = 0; i < 3; ++i) {
		if(1.0f < ouColor[i]) {
			ouColor /= ouColor[i];
		}
	}
	vec3 myPos = ouPos[0].xyz;
	if(ouPos[0].w == 0.0f) {
		gl_Position = unView * vec4(unViewPos + myPos * 50.0f + unBillboard * vec3(-1.0f, 1.0f, 0.0f), 1.0f);
		EmitVertex();
		gl_Position = unView * vec4(unViewPos + myPos * 50.0f + unBillboard * vec3(-1.0f, -1.0f, 0.0f), 1.0f);
		EmitVertex();
		gl_Position = unView * vec4(unViewPos + myPos * 50.0f + unBillboard * vec3(1.0f, 1.0f, 0.0f), 1.0f);
		EmitVertex();
		gl_Position = unView * vec4(unViewPos + myPos * 50.0f + unBillboard * vec3(1.0f, -1.0f, 0.0f), 1.0f);
		EmitVertex();
	} else {
		gl_Position = unView * vec4(myPos + unBillboard * vec3(-1.0f, 1.0f, 0.0f), 1.0f);
		EmitVertex();
		gl_Position = unView * vec4(myPos + unBillboard * vec3(-1.0f, -1.0f, 0.0f), 1.0f);
		EmitVertex();
		gl_Position = unView * vec4(myPos + unBillboard * vec3(1.0f, 1.0f, 0.0f), 1.0f);
		EmitVertex();
		gl_Position = unView * vec4(myPos + unBillboard * vec3(1.0f, -1.0f, 0.0f), 1.0f);
		EmitVertex();
	}

	EndPrimitive();

}
