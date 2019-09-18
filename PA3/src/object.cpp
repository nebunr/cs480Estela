#include "object.h"

Object::Object()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  orbit_angle = 2 * M_PI;
  rotation_angle = 2 * M_PI;

  orbit_speed = 2.0f;
  rotation_speed = 1.5f;

  orbit_dir = 1;
  rotation_dir = 1;

  translate_x = 0.0f;
  translate_y = 0.0f;
  translate_z = 0.0f;
  
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt) // move the object here
{
  rotation_angle += dt * M_PI/1000 * rotation_dir * rotation_speed;
  orbit_angle += dt * M_PI/500 * orbit_dir * orbit_speed;
  
  translate_x = 2 * cos( orbit_angle );
  translate_y = 0.0f;
  translate_z = 2 * sin( orbit_angle );

  model_rotate = glm::rotate(glm::mat4(1.0f), (rotation_angle), glm::vec3(0.0, 1.0, 0.0));
  model_translate = glm::translate(glm::mat4(1.0f), glm::vec3(translate_x, translate_y, translate_z));
  model = model_rotate * model_translate;
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Object::Input(char input)
{
  switch(input)
  {
    case 'q': // Keyboard inputs by char
      if (rotation_dir == -1) // Changes rotation dir
      {
        rotation_dir = 0; // Paused
      }
      else if (rotation_dir == 0)
      {
        rotation_dir = 1; // Clockwise
      }
      else if (rotation_dir == 1)
      {
        rotation_dir = -1; // Counter-Clockwise
      }
      break;
    case 'w':
      if (orbit_dir == -1) // Changes orbit dir
      {
        orbit_dir = 0; // Paused
      }
      else if (orbit_dir == 0)
      {
        orbit_dir = 1; // Counter-Clockwise
      }
      else if (orbit_dir == 1)
      {
        orbit_dir = -1; // Clockwise
      }
      break;
    case 'e':
      break;
    case 'r':
      break;
    case 'a':
      rotation_dir *= -1;
      break;
    case 's':
      orbit_dir *= -1;
      break;
    case 'd':
      rotation_dir *= -1;
      orbit_dir *= -1;
      break;
    case 'f':
      //rotation_speed /= 1.01;
      break;
    case '1': // Left Click
      rotation_dir *= -1;
      break;
    case '2': // Right Click
      orbit_dir *= -1;
      break;
    case '3': // Middle Click
      rotation_dir *= -1;
      orbit_dir *= -1;
      break;      
    default:
      break;
  }
}
