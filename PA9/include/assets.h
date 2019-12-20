#ifndef ASSETS_H
#define ASSETS_H

struct Config{
  char* vshader1_filename;
  char* fshader1_filename;
  char* vshader2_filename;
  char* fshader2_filename;
  char* vshader3_filename;
  char* fshader3_filename;
  char* window_name;
  int w;
  int h;
  char* ball_config;
  char* cube_config;
  char* cylinder_config;
  char* table_config;
  float ar;
  float ag;
  float ab;
  float ambient_intensity;
};

#endif
