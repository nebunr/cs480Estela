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
	glm::vec4 inital_rotation;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;
	bool is_dynamic;
        bool is_kinematic;
	char* shape;
};

class Object
{
  public:
    Object(char* object_config_filename);
    Object();
    ~Object();
    void Update(unsigned int dt, btDiscreteDynamicsWorld* dynamicsWorld);
    void ProcessInput(char input);
    void Render();

    void SetScore(int);
    void LoadModel(char* obj_filename);
    void ParseObjectConfig(char* object_config_filename);
    void LoadTexture(char* texture_filepath);

    GLuint getTexture();
    void SetTexture(GLuint text);

    glm::mat4 GetModel();

    btRigidBody* GetRigidBody();

    void LoadShape(char* shape); 

    float Scale();
    float GetMass();

    float GetShininess();
    glm::vec4 GetDiffuse();
    glm::vec4 GetAmbient();
    glm::vec4 GetSpecular(); 
    glm::vec3 GetLocation(); 
    int GetScore();
    void SetLocation(glm::vec3 lastLocation);

    void applyForce(btVector3 force);
  private:
    glm::mat4 model;
    glm::vec3 location;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<glm::vec3> normals;
    GLuint VB;
    GLuint IB;
    GLuint normalBuffer;
    GLuint texture;

    ObjectConfig cfg;

    int score;

    btCollisionShape* shape;
    btRigidBody* body;
    btDefaultMotionState* shapeMotionState;
    btTriangleMesh* triangle_mesh;
};

#endif /* OBJECT_H */
