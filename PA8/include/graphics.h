#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, Config cfg);
    void Update(unsigned int dt,char input,glm::vec2 mouseLocation);
    void Render();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

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
