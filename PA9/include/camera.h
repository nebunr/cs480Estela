#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    void Update(unsigned int dt, glm::vec2 mouseLocation);
    void Input(char input, unsigned int dt);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
  private:
    glm::mat4 projection;
    glm::mat4 view;
    int cameraLocation_x;
    int cameraLocation_y;
    int cameraLocation_z;
    float xLook;
    float yLook;
    //float zLook;
};

#endif /* CAMERA_H */
