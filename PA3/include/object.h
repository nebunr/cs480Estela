#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();
    void Input(char input);

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    glm::mat4 model_rotate;
    glm::mat4 model_translate;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float orbit_angle;
    float rotation_angle;
    
    float orbit_speed;
    float rotation_speed;
    
    int orbit_dir;
    int rotation_dir;

    float translate_x;
    float translate_y;
    float translate_z;
    
};

#endif /* OBJECT_H */
