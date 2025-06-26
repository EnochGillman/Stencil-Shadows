#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "glad/glad.h"

#include "get_file.hpp"

float length2(glm::vec3 v);

//Shader which does some light caching of uniforms
class Shader {
public:
	unsigned int ID;		//how we refrence the shader

	//for uniform caching
	std::vector<std::tuple<unsigned int, int*>> uniform1is;
	std::vector<std::tuple<unsigned int, float*>> uniform1fs;
	std::vector<std::tuple<unsigned int, glm::vec3*>> uniform3fs;
	std::vector<std::tuple<unsigned int, glm::vec3>> c_uniform3fs;
	std::vector<std::tuple<unsigned int, glm::vec4*>> uniform4fs;
	std::vector<std::tuple<unsigned int, glm::mat4*>> uniform4ms;
	std::vector<std::tuple<unsigned int, glm::mat3*>> uniform3ms;
	std::vector<unsigned int> c_texture2Ds;

	//use is called right before using the shader
	Shader(std::string vertex, std::string fragment);
	Shader(std::string vertex, std::string geometry, std::string fragment);
	Shader();
	void set(std::string vertex, std::string fragment);
	void set(std::string vertex, std::string geometry, std::string fragment);
	void use();
	~Shader();

	//called right after creating the shader
	void add_uniform1i(const char* name, int* data);
	void add_uniform1f(const char* name, float* data);
	void use_uniform1f(const char* name, float* data);
	void add_uniform3f(const char* name, glm::vec3* data);
	void use_uniform3f(const char* name, glm::vec3* data);
	void add_const_uniform3f(const char* name, glm::vec3 data);
	void add_uniform4f(const char* name, glm::vec4* data);
	void use_uniform4f(const char* name, glm::vec4* data);
	void add_uniform3m(const char* name, glm::mat3* data);
	void add_uniform4m(const char* name, glm::mat4* data);
	void add_const_texture2D(const char* name, unsigned int data);

};

#endif
