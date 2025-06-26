#include "shader.hpp"

float length2(glm::vec3 v) {
	return glm::dot(v, v);
}

Shader::Shader(std::string vertex, std::string fragment) {
	set(vertex, fragment);
}
Shader::Shader(std::string vertex, std::string geometry, std::string fragment) {
	set(vertex, geometry, fragment);
}
Shader::Shader() {}

void Shader::set(std::string vertex, std::string fragment) {
	int success;
	char infoLog[512];

	std::string vertexSource = get_file_contents(vertex);
	std::string fragmentSource = get_file_contents(fragment);
	char* vS = &vertexSource[0];
	char* fS = &fragmentSource[0];

	//create shader object with an ID of "vertexShader"
	//attach shader source to shader object
	//compile "vertexShader"
	// print compile errors if any
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vS, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	//create shader object with an ID of "vertexShader"
	//attach shader source to shader object
	//compile "vertexShader"
	//print compile errors if any
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fS, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	//create a shader program to link the vertex and fragment shaders
	ID = glCreateProgram();
	//attach the shaders to the program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	//link and use the program
	glLinkProgram(ID);
	glUseProgram(ID);

	//once linked, these shaders are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void Shader::set(std::string vertex, std::string geometry, std::string fragment) {
	int success;
	char infoLog[512];

	std::string vertexSource = get_file_contents(vertex);
	std::string geometrySource = get_file_contents(geometry);
	std::string fragmentSource = get_file_contents(fragment);
	char* vS = &vertexSource[0];
	char* gS = &geometrySource[0];
	char* fS = &fragmentSource[0];

	//create shader object with an ID of "vertexShader"
	//attach shader source to shader object
	//compile "vertexShader"
	// print compile errors if any
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vS, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &gS, NULL);
	glCompileShader(geometryShader);
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	//create shader object with an ID of "vertexShader"
	//attach shader source to shader object
	//compile "vertexShader"
	//print compile errors if any
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fS, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	//create a shader program to link the vertex and fragment shaders
	ID = glCreateProgram();
	//attach the shaders to the program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, geometryShader);
	glAttachShader(ID, fragmentShader);
	//link and use the program
	glLinkProgram(ID);
	glUseProgram(ID);

	//once linked, these shaders are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(ID);
	for(size_t i = 0; i < c_texture2Ds.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, c_texture2Ds[i]);
	}
	for(std::tuple<unsigned int, int*> i : uniform1is) {
		glUniform1i(std::get<0>(i), *std::get<1>(i));
	}
	for(std::tuple<unsigned int, float*> i : uniform1fs) {
		glUniform1f(std::get<0>(i), *std::get<1>(i));
	}
	for(std::tuple<unsigned int, glm::vec3*> i : uniform3fs) {
		glUniform3fv(std::get<0>(i), 1, &(std::get<1>(i)->x));
	}
	for(std::tuple<unsigned int, glm::vec4*> i : uniform4fs) {
		glUniform4fv(std::get<0>(i), 1, &(std::get<1>(i)->x));
	}
	for(std::tuple<unsigned int, glm::vec3> i : c_uniform3fs) {
		glUniform3fv(std::get<0>(i), 1, &(std::get<1>(i).x));
	}
	for(std::tuple<unsigned int, glm::mat3*> i : uniform3ms) {
		glUniformMatrix3fv(std::get<0>(i), 1, GL_FALSE, glm::value_ptr(*std::get<1>(i)));
	}
	for(std::tuple<unsigned int, glm::mat4*> i : uniform4ms) {
		glUniformMatrix4fv(std::get<0>(i), 1, GL_FALSE, glm::value_ptr(*std::get<1>(i)));
	}
}

void Shader::add_uniform1i(const char* name, int* data) {
	uniform1is.push_back(std::make_tuple(
		glGetUniformLocation(ID, name),
					     data
	));
}

void Shader::add_uniform1f(const char* name, float* data) {
	uniform1fs.push_back(std::make_tuple(
		glGetUniformLocation(ID, name),
					     data
	));
}

void Shader::use_uniform1f(const char* name, float* data) {
	glUniform1f(glGetUniformLocation(ID, name), *data);
}

void Shader::add_uniform3f(const char* name, glm::vec3* data) {
	uniform3fs.push_back(std::make_tuple(
		glGetUniformLocation(ID, name),
					     data
	));
}

void Shader::use_uniform3f(const char* name, glm::vec3* data) {
	glUniform3fv(glGetUniformLocation(ID, name), 1, &data->x);
}

void Shader::add_const_uniform3f(const char* name, glm::vec3 data) {
	c_uniform3fs.push_back(std::make_tuple(
		glGetUniformLocation(ID, name),
					       data
	));
}

void Shader::add_uniform4f(const char* name, glm::vec4* data) {
	uniform4fs.push_back(std::make_tuple(
		glGetUniformLocation(ID, name),
					     data
	));
}

void Shader::use_uniform4f(const char* name, glm::vec4* data) {
	glUniform4fv(glGetUniformLocation(ID, name), 1, &data->x);
}

void Shader::add_uniform3m(const char* name, glm::mat3* data) {
	uniform3ms.push_back(std::make_tuple(
		glGetUniformLocation(ID, name),
					     data
	));
}

void Shader::add_uniform4m(const char* name, glm::mat4* data) {
	uniform4ms.push_back(std::make_tuple(
		glGetUniformLocation(ID, name),
					     data
	));
}
void Shader::add_const_texture2D(const char* name, unsigned int data) {
	glUseProgram(ID);
	glUniform1i(
		glGetUniformLocation(ID, name),
		c_texture2Ds.size()
	);
	c_texture2Ds.push_back(data);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}
