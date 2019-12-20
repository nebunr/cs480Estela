#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, Config cfg);
    void Update(unsigned int dt,char input,glm::vec2 mouseLocation);
    void Render();
    void HandleFlatEarth();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Object *m_cube;
    std::vector<Object*> objects;
    std::vector<Object*> followable_objects;
    int selected_index;
    int following_index;
    
    bool flat_earth;
};

#endif /* GRAPHICS_H */
