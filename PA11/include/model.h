#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "vertex.h"
#include "graphics_headers.h"

class Model{
	public:
		Model(char* model_filename);
		Model();
		~Model();
		void Render();
		btTriangleMesh* GetTriangleMesh();
	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		GLuint vertex_buffer;
		GLuint index_buffer;
		btTriangleMesh* triangle_mesh;

};

#endif //MODEL_H
