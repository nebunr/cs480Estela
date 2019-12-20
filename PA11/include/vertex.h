//--------------------------------------------------------------------
//
// vertex.h
// Vertex struct containing the vertex, normal coordinates, and
// texture coordinates for a model
//
//--------------------------------------------------------------------

#ifndef VERTEX_H
#define VERTEX_H

#include "graphics_headers.h"

struct Vertex{
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 texture_coordinates;

	Vertex(glm::vec3 v, glm::vec3 n, glm::vec2 t): vertex(v), normal(n), texture_coordinates(t) {}
};

#endif //VERTEX_H
