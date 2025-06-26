#version 460 core
out vec4 FragColor;

in vec2 ouTexCoord;

uniform sampler2D unScreen;


void main() {
	FragColor = texture(unScreen, ouTexCoord);

	//Change this to increase color levels
	int comp = 8;

	float bright = round((FragColor.x * 0.2126 + FragColor.y * 0.7152 + FragColor.z * 0.0722) * comp + 0.25) / comp;

	float red = (FragColor.x + bright) / 2;
	float green = (FragColor.y + bright) / 2;
	float blue = (FragColor.z + bright) / 2;

	float gray = red * 0.2126 + green * 0.7152 + blue * 0.0722;
	float sat = 3;

	red = red * (1 + sat) - gray * sat;
	green = green * (1 + sat) - gray * sat;
	blue = blue * (1 + sat) - gray * sat;

	FragColor = vec4(red, green, blue, 1.0f);
}

// void main()
// {
// 	//Change this number to increase the pixel size.
// 	float pixelSize = 1.0;
//
// 	float red    = 0.0;
// 	float green    = 0.0;
// 	float blue    = 0.0;
//
// 	float2 pos = floor(GetCoordinates() * GetResolution() / pixelSize) * pixelSize * GetInvResolution();
//
// 	float4 c0 = SampleLocation(pos);
//
// 	//Change this to increase color levels
// 	int comp = 4;
//
// 	float bright = round((c0.x + c0.y + c0.z) / 3 * comp + 0.25) / comp;
//
// 	red = (c0.x + bright) / 2;
// 	green = (c0.y + bright) / 2;
// 	blue = (c0.z + bright) / 2;
//
// 	float gray = (red + green + blue) / 3;
// 	float sat = 1;
//
// 	red = red * (1 + sat) - gray * sat;
// 	green = green * (1 + sat) - gray * sat;
// 	blue = blue * (1 + sat) - gray * sat;
//
// 	SetOutput(float4(red, green, blue, c0.a));
// }
