#include "texture.h"

Texture::Texture() {}

Texture::~Texture() {}

Texture::Texture(char* texture_filename){
	Magick::Image* image;
	Magick::Blob blob;

	//Initialize imagemagick
	Magick::InitializeMagick(NULL);

	//load the image data into a blob and bind textures
	image = new Magick::Image(texture_filename);
	image->write(&blob,"RGBA");
	glGenTextures(1,&texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Render(){
	glBindTexture(GL_TEXTURE_2D, texture);
}
