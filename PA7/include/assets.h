#ifndef ASSETS_H
#define ASSETS_H

struct Config{
  char* vshader_filename;
  char* fshader_filename;
  char* window_name;
  int w;
  int h;
  char* root_planet_filename;
  float orbit_scale;
  float planet_scale;
  float time_scale; 
};
#endif
