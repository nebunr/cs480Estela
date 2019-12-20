//--------------------------------------------------------------------
//
// config.h
// Config struct containing any meta config information, such as
// window sizing and naming
//
//--------------------------------------------------------------------

#ifndef CONFIG_H
#define CONFIG_H

struct Config{
	char* window_name;
	char* vshader_filename;
	char* fshader_filename;
	char* scoreboard_filename;
	int w;
	int h;
	
	float ar;
	float ag;
	float ab;
	float ambient_intensity;
	float specular_intensity;

	float sr;
	float sg;
	float sb;
	float sx;
	float sy;
	float sz;
	float spotlightIntensity;
	float spotlightCutoff;
};

#endif //CONFIG_H
