#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <vector>
#include "config.h"
#include "graphics_headers.h"

class Shader{
	public:
		Shader();
		~Shader();
		bool Initialize();
		void Enable();
		bool AddShader(GLenum ShaderType, char* filename);
		bool Finalize();
		GLint GetUniformLocation(const char* uniform_name);
	private:
		GLuint shader_program;
		std::vector<GLuint> shaders;
};

#endif //SHADER_H
