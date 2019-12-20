#ifndef TEXTURE_H
#define TEXTURE_H

#include "graphics_headers.h"

#include <Magick++.h>

class Texture{
	public:
		Texture(char* texture_filename);
		Texture();
		~Texture();
		void Render();
	private:
		GLuint texture;
};

#endif //TEXTURE_H
