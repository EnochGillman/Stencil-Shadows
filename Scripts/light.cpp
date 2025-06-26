#include "light.hpp"

unsigned int Light::VAO;
unsigned int Light::VBO;
Shader Light::shader;
glm::vec3 Light::p;

Light::Light(
	glm::vec4 p,
	glm::vec3 d,
	glm::vec3 c,
	GLfloat di,
	GLfloat cO,
	GLfloat cS
)
: pos(p),dir(d),color(c),dist(di),cutOff(cO),cutSmo(cS) {}

void Light::init(glm::vec3* pos, glm::mat4* view, glm::mat3* billboard) {
	p = glm::vec3(0.0f);
	shader.set(
		"Resources/Shaders/Model/debug_light.vert",
		"Resources/Shaders/Model/debug_light.geom",
		"Resources/Shaders/Model/debug_light.frag"
	);
	shader.add_uniform3f("unViewPos", pos);
	shader.add_uniform4m("unView", view);
	shader.add_uniform3m("unBillboard", billboard);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &p[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
}

void Light::use() {
	shader.use();
	glUniform3fv(glGetUniformLocation(shader.ID, "unColor"), 1, &color[0]);
	glUniform4fv(glGetUniformLocation(shader.ID, "unPos"), 1, &pos[0]);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}
void Light::bind(Shader* shader) {
	shader->use_uniform4f("unLight.pos", &pos);
	shader->use_uniform3f("unLight.dir", &dir);
	shader->use_uniform3f("unLight.col", &color);
	shader->use_uniform1f("unLight.dist", &dist);
	shader->use_uniform1f("unLight.cutOff", &cutOff);
	shader->use_uniform1f("unLight.cutSmo", &cutSmo);
}
