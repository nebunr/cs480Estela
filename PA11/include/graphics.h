#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "spotlight.h"
#include "physics.h"
//#include "audio.h"

class Graphics{
	public:
		Graphics();
		~Graphics();
		bool Initialize(int w, int h, Config cfg);
		void Update(unsigned int dt, KeyHandler* key_handler, bool& died, bool& win, int level);
		void Render();
		void Input(char input);
		char* ErrorString(GLenum error);
		void Reset(int level);
	private:

		Camera* camera;
		std::vector<Shader*> shaders;

		GLint projection_matrix;
		GLint view_matrix;
		GLint model_matrix;
		GLint model_view_matrix;
		GLint m_lightPosition;
		GLint m_shininess;
		GLint m_ambient;
		GLint m_diffuse;
		GLint m_specular;
		GLint m_ambient_color;
		GLint m_ambient_intensity;
		GLint m_spotlight_position;
		GLint m_spotlight_direction;
		GLint m_spotlight_cutoff;
		GLint m_spotlight_color;
		GLint m_spotlight_intensity;
		GLint m_viewPos;
		GLint m_specular_intensity;

		Spotlight spotlight;
		glm::vec3 ambient_light_color;
		float ambient_light_intensity;
		float specular_intensity;
		unsigned int shader_index;
		float cutOffDivider;

		PhysicsWorld* physics_world;

		std::vector<Object*> objects;
};

#endif //GRAPHICS_H
