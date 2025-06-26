#include "camera.hpp"

unsigned int Camera::VAO;
unsigned int Camera::VBO;
unsigned int Camera::attachments[4];

Camera::Camera(int* w, int* h, float f, glm::vec3* a)
: width(w), height(h), fov(f) {
	clipNear = 0.1f;
	clipFar = 1000.0f;

	ambient = a;

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, *width, *height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	attachments[0] = GL_COLOR_ATTACHMENT0+0;
	attachments[1] = GL_COLOR_ATTACHMENT0+1;
	attachments[2] = GL_COLOR_ATTACHMENT0+2;
	attachments[3] = GL_COLOR_ATTACHMENT0+3;
	glDrawBuffers(4, attachments);

	glGenTextures(1, &unlitTexture);
	glBindTexture(GL_TEXTURE_2D, unlitTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, unlitTexture, 0);

	glGenTextures(1, &fogTexture);
	glBindTexture(GL_TEXTURE_2D, fogTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, *width, *height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fogTexture, 0);

	glGenTextures(1, &litTexture);
	glBindTexture(GL_TEXTURE_2D, litTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, *width, *height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, litTexture, 0);

	glGenTextures(1, &debugTexture);
	glBindTexture(GL_TEXTURE_2D, debugTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, debugTexture, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

void Camera::init() {
	std::vector<float> vertices {
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2));
}

//mouse position, foward/back, left/right, up/down, scroll
void Camera::use(float mosX, float mosY, float sc, float fb, float lr, float ud) {
	fov += sc;

	rotX = std::fmod(rotX - mosX * 0.002f, PI * 2.0f);
	rotY = rotY - mosY * 0.002f;
	if(PI*0.499f < std::abs(rotY)) {
		rotY = PI*0.499f * rotY / std::abs(rotY);
	}

	look.x = std::sin(rotX) * glm::cos(rotY);
	look.y = std::sin(rotY);
	look.z = std::cos(rotX) * glm::cos(rotY);
	if(up.x != 0.0f || up.y != 1.0f || up.z != 0.0f) {
		look = glm::rotate(
			look,
			-std::acos(glm::dot(up, glm::vec3(0.0f, 1.0f, 0.0f))), //angle
			glm::cross(up, glm::vec3(0.0f, 1.0f, 0.0f)) //position
		);
	}

	right = glm::normalize(glm::cross(look, up));

	if(0 < std::abs(fb) || 0 < std::abs(lr)) {
		vel +=
			glm::normalize(
				fb * glm::normalize(glm::cross(right, up))
				+ lr * right
			) * speed;
	}
	vel += ud * up * speed;
	vel *= 0.9f;
	pos += vel;

	//use vectors to create the matrix
	view =
		glm::lookAt(
			pos,
			pos + look,
			up
		);
	screen =
		glm::perspective(
			glm::radians(70.0f),
			float(*width)/(*height),
			clipNear,
			clipFar
		)
		* glm::lookAt(
			-look,
			glm::vec3(0.0f),
			up
		);
	billboard = glm::mat4(glm::inverse(glm::mat3(view)));
	view =
		glm::perspective(
			glm::radians(fov),
			float(*width)/(*height),
			clipNear,
			clipFar
		)
		* view;
}

void Camera::unlitPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glEnable(GL_DEPTH_TEST);
	glStencilMask(0xFF);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	float a[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glClearTexImage(unlitTexture, 0, GL_RGB, GL_FLOAT, &a);
	glClearTexImage(fogTexture, 0, GL_RGB, GL_FLOAT, &a);
	glClearTexImage(litTexture, 0, GL_RGB, GL_FLOAT, 0);
	glClearTexImage(debugTexture, 0, GL_RGB, GL_FLOAT, 0);
	glViewport(0, 0, *width, *height);
}

void Camera::show(Shader* shader) {
	shader->use();

	glBindVertexArray(VAO);
	glDisable(GL_DEPTH_TEST);
	glUniform1i(
		glGetUniformLocation(shader->ID, "unScreen"),
		0
	);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, unlitTexture);
	glUniform1i(
		glGetUniformLocation(shader->ID, "unLighting"),
		 1
	);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, litTexture);
	glUniform1i(
		glGetUniformLocation(shader->ID, "unFog"),
		 2
	);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fogTexture);
	glUniform1i(
		glGetUniformLocation(shader->ID, "unDebug"),
		 3
	);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, debugTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
