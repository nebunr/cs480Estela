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
  cameraLocation_x = 0;
  cameraLocation_y = 44;
  cameraLocation_z = 60;
  //xLook = 45;
  //yLook = 180;
  //zLook = 0;
  //front = {0,0,0};
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  view = glm::lookAt( glm::vec3(cameraLocation_x, cameraLocation_y, cameraLocation_z), //Eye Position. Default (0,5,10)
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
  /*
  xLook += mouseLocation.x;
  yLook += mouseLocation.y; //normal/non-inverted vertical look

  front.x = cos(glm::radians(yLook)) * cos(glm::radians(xLook));
  front.y = sin(glm::radians(yLook));
  front.z = cos(glm::radians(yLook)) * sin(glm::radians(xLook));
  
  front = glm::normalize(front);
  
  front.x += cameraLocation_x;
  front.y += cameraLocation_y;
  front.z += cameraLocation_z;
  */

  //boundry for cameraLocation
  /*
  if(cameraLocation_x > 110)
  {
    cameraLocation_x = 110;
  }
  if(cameraLocation_x < 10)
  {
    cameraLocation_x = 10;
  }
  */
  view = glm::lookAt( glm::vec3(cameraLocation_x, cameraLocation_y, cameraLocation_z),
                      glm::vec3(0.0, 0.0, 0.0),
                      glm::vec3(0.0, 1.0, 0.0));
  
}

void Camera::Input(char input, unsigned int dt)
{
  int movement = 5;// * (dt * 0.001);
  glm::vec3 strafe(view[0][0], view[1][0], view[2][0]);
  glm::vec3 height(view[0][1], view[1][1], view[2][1]);
  glm::vec3 fwd(view[0][2], view[1][2], view[2][2]);
  fwd.y *= -1;
  fwd.z *= -1;
  switch(input)
  {
    /*
    case 3:
      xLook = 45;
      yLook = 180;
      break;
    case 'i': //up
      /*
      cameraLocation_x += movement * height.x;
      cameraLocation_y += movement * height.y;
      cameraLocation_z -= movement * height.z;
      
      //cameraLocation_y += 10;
      std::cout << "X: " << cameraLocation_x << std::endl;
      std::cout << "Y: " << cameraLocation_x << std::endl;
      std::cout << "Z: " << cameraLocation_z << std::endl;
      cameraLocation_x -= movement * fwd.x;
      cameraLocation_y += movement * fwd.y;
      cameraLocation_z += movement * fwd.z;

      break;
    case 'j': //left
      std::cout << "X: " << cameraLocation_x << std::endl;
      std::cout << "Y: " << cameraLocation_x << std::endl;
      std::cout << "Z: " << cameraLocation_z << std::endl;
      cameraLocation_x -= movement * strafe.x;
      cameraLocation_z -= movement * strafe.z;
      break;
    case 'l': //right
      std::cout << "X: " << cameraLocation_x << std::endl;
      std::cout << "Y: " << cameraLocation_x << std::endl;
      std::cout << "Z: " << cameraLocation_z << std::endl;
      cameraLocation_x += movement * strafe.x;
      cameraLocation_z += movement * strafe.z;
      break;
    case 'k': //down
      std::cout << "X: " << cameraLocation_x << std::endl;
      std::cout << "Y: " << cameraLocation_x << std::endl;
      std::cout << "Z: " << cameraLocation_z << std::endl;
      cameraLocation_x += movement * fwd.x;
      cameraLocation_y -= movement * fwd.y;
      cameraLocation_z -= movement * fwd.z;
      break;
    default:
      break;
    */
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

glm::vec3 Camera::GetPos()
{
  return glm::vec3(cameraLocation_x, cameraLocation_y, cameraLocation_z);
}
