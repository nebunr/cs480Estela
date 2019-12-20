#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "spotlight.h"
#include "object.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, Config cfg);
    void Update(unsigned int dt, char input, glm::vec2 mouseLocation);
    void Render();
    void Input(char input);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    std::vector<Shader*> m_shaders;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_modelViewMatrix;
    GLint m_lightPosition;
    GLint m_shininess;
    GLint m_ambient;
    GLint m_diffuse;
    GLint m_specular;
    GLint m_ambient_color;
    GLint m_ambient_intensity;
    GLint m_spotlight_position;
    GLint m_spotlight_direction;
    GLint m_spotlight_cutoff;
    GLint m_spotlight_color;
    GLint m_spotlight_intensity;

    Spotlight spotlight;
    glm::vec3 ambient_light_color;
    float ambient_light_intensity;
    unsigned int shader_index;

    Object* m_test;
    Object* m_ball;
    Object* m_cube;
    Object* m_cylinder;
    Object* m_table;
    
    btDiscreteDynamicsWorld* dynamics_world;
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collision_config;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;

    std::vector<Object*> objects;
};

#endif /* GRAPHICS_H */
