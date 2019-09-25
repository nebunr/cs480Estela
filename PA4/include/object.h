#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <fstream>
#include <sstream>
#include "graphics_headers.h"
#include "assets_headers.h"

class Object
{
  public:
    Object(assets asset);
    ~Object();
    void ReadObj(assets asset);
    void Update(unsigned int dt, int object_num, glm::mat4 planet);
    void Render();

    glm::mat4 GetModel();
    glm::mat4 GetLocation();

  private:
    glm::mat4 model;
    glm::mat4 model_rotate;
    glm::mat4 model_translate;
    glm::mat4 model_location;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
};

#endif /* OBJECT_H */
