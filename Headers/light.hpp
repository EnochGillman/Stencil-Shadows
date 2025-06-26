#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "model.hpp"

class Light {
	static unsigned int VAO, VBO;
	static Shader shader;	//global
	static glm::vec3 p;
public:
	glm::vec4 pos;
	glm::vec3 dir;
	glm::vec3 color;
	GLfloat dist;
	GLfloat cutOff;
	GLfloat cutSmo;
	Light(
		glm::vec4 p,
		glm::vec3 d,
		glm::vec3 c,
		GLfloat di,
		GLfloat cO,
		GLfloat cS
	);
	static void init(glm::vec3* pos, glm::mat4* view, glm::mat3* billboard);
	void use();
	void bind(Shader* shader);
};

#endif
