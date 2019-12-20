#include "camera.h"
glm::vec3 front; // Yes, it's global. We got memory corruption if it was in camera.h
Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  // Init cameraLocation for update
  cameraLocation_x = 90;
  cameraLocation_y = 1;
  cameraLocation_z = 0;
  xLook = 45;
  yLook = 180;
  //zLook = 0;
  front = {0,0,0};
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  view = glm::lookAt( glm::vec3(0, 9, -17), //Eye Position. Default (0,50,8)
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 2000.0f); //Distance to the far plane, Default 100f
  return true;
}

void Camera::Update(unsigned int dt, glm::vec2 mouseLocation)
{
  xLook += mouseLocation.x;
  yLook += mouseLocation.y; //normal/non-inverted vertical look

  front.x = cos(glm::radians(yLook)) * cos(glm::radians(xLook));
  front.y = sin(glm::radians(yLook));
  front.z = cos(glm::radians(yLook)) * sin(glm::radians(xLook));
  
  front = glm::normalize(front);
  
  front.x += cameraLocation_x;
  front.y += cameraLocation_y;
  front.z += cameraLocation_z;

  view = glm::lookAt( glm::vec3(cameraLocation_x, cameraLocation_y, cameraLocation_z),
                      front,
                      glm::vec3(0.0, 1.0, 0.0));
}

void Camera::Input(char input, unsigned int dt)
{
  int movement = 5;// * (dt * 0.001);
  glm::vec3 fwd(view[0][2], view[1][2], view[2][2]);
  fwd.y *= -1;
  fwd.z *= -1;
  glm::vec3 strafe(view[0][0], view[1][0], view[2][0]);
  switch(input)
  {
    case 1:
      cameraLocation_y += 10;
      break;
    case 2:
      cameraLocation_y -= 10;
      break;
    case 3:
      xLook = 45;
      yLook = 180;
      break;
    case '^':
      cameraLocation_x -= movement * fwd.x;
      cameraLocation_y += movement * fwd.y;
      cameraLocation_z += movement * fwd.z;
      break;
    case '<':
      cameraLocation_x -= movement * strafe.x;
      cameraLocation_z -= movement * strafe.z;
      break;
    case '>':
      cameraLocation_x += movement * strafe.x;
      cameraLocation_z += movement * strafe.z;
      break;
    case 'V':
      cameraLocation_x += movement * fwd.x;
      cameraLocation_y -= movement * fwd.y;
      cameraLocation_z -= movement * fwd.z;
      break;
    default:
      break;
  }
}

glm::mat4 Camera::GetProjection()
{	
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}
