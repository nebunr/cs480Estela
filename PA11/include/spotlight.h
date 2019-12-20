#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "graphics_headers.h"

struct Spotlight{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
	float cutoff;
};

#endif