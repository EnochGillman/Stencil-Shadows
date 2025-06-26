#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_FORCE_RADIANS	1
#define GLM_ENABLE_EXPERIMENTAL
#define PI			float(3.14159265358979323846)

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "shader.hpp"

class Camera {
	static unsigned int VAO, VBO;
	static unsigned int attachments[4];
	int* width;
	int* height;
	float rotX {-PI*0.5};
	float rotY {0.0f};
	glm::vec3* ambient;

	float speed {0.1f};
public:
	unsigned int frameBuffer, renderBuffer, unlitTexture, litTexture, debugTexture, fogTexture;
	glm::vec3 pos {glm::vec3(0.0f)};
	glm::vec3 vel {glm::vec3(0.0f)};
	glm::vec3 look;
	glm::vec3 right;
	glm::vec3 up {glm::vec3(0.0f, 1.0f, 0.0f)};
	glm::mat4 view {glm::mat4(1.0f)};
	glm::mat4 screen {glm::mat4(1.0f)};
	glm::mat3 billboard {glm::mat3(1.0f)};
	float fov;
	float clipNear;
	float clipFar;

	Camera(int* w, int* h, float f, glm::vec3* a);
	static void init();

	void use(float mosX, float mosY, float fb, float lr, float ud, float sc);
	void unlitPass();
	void show(Shader* shader);
};

#endif
