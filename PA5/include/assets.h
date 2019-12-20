#ifndef ASSETS_H
#define ASSETS_H

struct ShaderFiles{
	char* vshader_filename;
	char* fshader_filename;
	char* config_filename;
	char* obj_filename;
	float scale;
	ShaderFiles(){
	  scale = 0.0;
	}
};

#endif
