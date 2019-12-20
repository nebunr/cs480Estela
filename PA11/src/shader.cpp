#include "shader.h"

Shader::Shader(){
	shader_program = 0;
}

Shader::~Shader(){
	for(unsigned int i = 0; i < shaders.size(); i++){
		glDeleteShader(shaders[i]);
	}
	if(shader_program != 0){
		glDeleteProgram(shader_program);
		shader_program = 0;
	}
}

bool Shader::Initialize(){
	shader_program = glCreateProgram();
	if(shader_program == 0){
		std::cerr << "Error creating shader program\n";
		return false;
	}
	return true;
}

bool Shader::AddShader(GLenum ShaderType, char* filename){
	std::string s;
	std::ifstream shader_file(filename);
	if(!shader_file.good()){
		if(ShaderType == GL_VERTEX_SHADER){
			printf("Vertex shader file is invalid: %s\n",filename);
			return false;
		}
		else{
			printf("Fragment shader file is invalid: %s\n",filename);
			return false;
		}
	}
	s = std::string(std::istreambuf_iterator<char>(shader_file),std::istreambuf_iterator<char>());
	GLuint ShaderObj = glCreateShader(ShaderType);
	if(ShaderObj == 0){
		std::cerr << "Error creating shader type " << ShaderType << std::endl;
		return false;
	}

	//Save the shader object
	shaders.push_back(ShaderObj);

	const GLchar* p[1];
	p[0] = s.c_str();
	GLint Lengths[1] = { (GLint)s.size() };
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if(!success){
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling: " << InfoLog << std::endl;
		return false;
	}

	glAttachShader(shader_program, ShaderObj);
	return true;
}

bool Shader::Finalize(){
	GLint success = 0;
	GLchar ErrorLog[1024] = {0};
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(success == 0){
		glGetProgramInfoLog(shader_program, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		return false;
	}
	glValidateProgram(shader_program);
	glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shader_program, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		return false;
	}
	//delete the intermediate shader objects
	for(unsigned int i = 0; i < shaders.size(); i++){
		glDeleteShader(shaders[i]);
	}
	shaders.clear();
	return true;
}

void Shader::Enable(){
	glUseProgram(shader_program);
}
GLint Shader::GetUniformLocation(const char* uniform_name){
	GLuint Location = glGetUniformLocation(shader_program, uniform_name);
	if(Location == INVALID_UNIFORM_LOCATION){
		fprintf(stderr, "Warning! Unable to ge tthe location of uniform '%s'\n", uniform_name);
	}
	return Location;
}

