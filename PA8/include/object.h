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
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "graphics_headers.h"
#include "assets.h"

struct ObjectConfig{
	float restitution;
	float friction;
	float mass;
	float scale;
	float radius;
	float width;
	float length;
	float height;
	glm::vec3 inital_rotation;
	bool is_dynamic;
  bool is_kinematic;
	char* shape;
};

class Object
{
  public:
    Object(char* object_config_filename, btDiscreteDynamicsWorld* dynamics_world);
    Object(btDiscreteDynamicsWorld* dynamics_world);
    ~Object();
    void Update(unsigned int dt, btDiscreteDynamicsWorld* dynamics_world);
    void ProcessInput(char input);
    void Render();

    void LoadModel(char* obj_filename);
    void ParseObjectConfig(char* object_config_filename);
    void LoadTexture(char* texture_filepath);

    GLuint getTexture();
    void SetTexture(GLuint text);

    glm::mat4 GetModel();

    btRigidBody* GetRigidBody();

    void LoadShape(char* shape); 

    float Scale(); 
  private:
    glm::mat4 model;
    glm::vec3 location;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint texture;

    ObjectConfig cfg;

    btCollisionShape* shape;
    btRigidBody* body;
    btDefaultMotionState* shapeMotionState;
    btTriangleMesh* triangle_mesh;
};

#endif /* OBJECT_H */
