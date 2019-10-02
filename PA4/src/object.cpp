#include "object.h"

Object::Object(assets asset)
{  
  /*
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
  */
  ReadObj(asset);

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }
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

void Object::ReadObj(assets asset)
{
  std::ifstream fileObject;
  fileObject.open(asset.bFile);  
  if(!(fileObject.is_open()))
  {
    std::cerr << "Cannot open .obj file.\n";
    return;
  }

  std::string lineVar; // Read through lines. [0] is the start of a newline
  std::string readIn; // Used to get through the file


  while(!fileObject.eof())
  {
    std::getline(fileObject, readIn, ' ');

    if(readIn.compare("v") == 0)
    {
      glm::vec3 vertex;
      glm::vec3 color;
      color = {0.0f, 0.5f, 0.0f}; // lime green
      fileObject >> vertex.x;
      fileObject >> vertex.y;
      fileObject >> vertex.z;
      Vertices.push_back({vertex, color});
      std::getline(fileObject, readIn, '\n');
    }
    else if(readIn.compare("f") == 0)
    {
      unsigned int x, y, z;
      std::getline(fileObject, readIn);
      std::stringstream ss(readIn);
      if(readIn.find("//") != std::string::npos)
      {
        ss >> x;
        ss >> readIn;
        ss >> y;
        ss >> readIn;
        ss >> z;
        ss >> readIn;
      }
      else if (readIn.find("/") != std::string::npos)
      {
        ss >> x;
        ss >> readIn;
        ss >> y;
        ss >> readIn;
        ss >> z;
        ss >> readIn;
      }
      else
      {
        ss >> x;
        ss >> y;
        ss >> z;
      }
      Indices.push_back(x);
      Indices.push_back(y);
      Indices.push_back(z);
    }
    else
    {
      std::getline(fileObject, readIn, '\n');
    }
  }
  fileObject.close();
}

void Object::Update(unsigned int dt, int object_num, glm::mat4 planet) // move the object here
{
  // Cleared most of the function for PA4
  model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
}

glm::mat4 Object::GetModel()
{
  return model;
}
glm::mat4 Object::GetLocation()
{
  return model_location;
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