#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "object.h"
class Camera{
	public:
		Camera();
		~Camera();
		bool Initialize(int w, int h);
		void Update(unsigned int dt,KeyHandler* key_handler, Object* player);
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetViewMatrix();
		glm::vec3 GetGravity();
		glm::vec3 GetPos();
		void Reset();
	private:
		glm::mat4 projection_matrix;
		glm::mat4 view_matrix;
		glm::vec3 location;
		glm::vec3 direction;
		glm::vec3 orientation;
		float angle;
		float pitch;
};

#endif //CAMERA_H
