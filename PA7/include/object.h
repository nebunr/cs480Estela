#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include <tinyxml2.h>
#include <Magick++.h>

#include "graphics_headers.h"
#include "assets.h"

struct ObjectConfig{
	std::string name;

	float orbit_speed;
	float orbit_angle;
	int orbit_direction;
	float orbit_distance;
	bool orbit_paused;

	float rotation_speed;
	float rotation_angle;
	int rotation_direction;
	bool rotation_paused;

	float scale;

	char texture_filepath[256];

	glm::vec3 orbit_axis;
	glm::vec3 rotation_axis;

	ObjectConfig(){
	  name = " ";
	  orbit_speed = 0;
	  orbit_angle = 0;
	  orbit_distance = 2;
	  orbit_direction = 1;
	  orbit_axis = glm::vec3(0.0,1.0,0.0);
	  orbit_paused = false;
	  rotation_speed = 0;
	  rotation_angle = 0;
	  rotation_direction = 1;
	  rotation_axis = glm::vec3(0.0,1.0,0.0);
	  rotation_paused = false;
	  scale = 1;
	  strcpy(texture_filepath,"\0");
	}
};

class Object
{
  public:
    static float orbit_scale;
    static float planet_scale;
    static float time_scale;

    Object();
    Object(char* object_config_filename);
    ~Object();
    void Update(unsigned int dt, bool flat_earth);
    void processInput(char input);
    void Input(char input);
    void Render();

    std::vector<Object*> getSatelites();
    void setSatelites(std::vector<Object*> vectors);

    Object* getParent();
    void setParent(Object* p);

    std::string getName();

    void parseObjFile(char* obj_filename);
    void parseObjectConfig(char* object_config_filename);
    void loadTexture(char* texture_filepath);

    GLuint getTexture();
    void setTexture(GLuint text);

    glm::mat4 GetModel();
    glm::mat4 GetLocation();

    bool isSelected();
    void Select();
    void Deselect();

    float getScale();
  
  private:
    glm::mat4 model;
    glm::mat4 location;
    bool selected;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint m_texture;
    std::vector<Object*> satelites;
    Object* parent;

    ObjectConfig config;
};

#endif /* OBJECT_H */
