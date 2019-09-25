#include "object.h"

Object::Object(assets asset)
{  
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
  std::ifstream fileObj;
  fileObj.open(asset.bFile);  

  std::string lineVar; // Read through lines. [0] is the start of a newline
  std::string temp; // Used to get through the file

  while(!fileObj.eof())
  {
    fileObj >> lineVar;
    while(fileObj.peek() == '\n' || fileObj.peek() == ' ')
    {
      fileObj.get();
    }
    // Use these if-else statements once you reach a newline
    // comments
    if(lineVar[0] == '#')
    {
      std::getline(fileObj, temp);
    }
    // object name (ex. Cube)
    else if(lineVar[0] == 'o')
    {
      std::getline(fileObj, temp);
    }
    // vertex
    else if(lineVar[0] == 'v')
    {
      glm::vec3 vertex;
      glm::vec3 color;
      color = {0.0f, 0.5f, 0.0f}; // lime green
      fileObj >> vertex.x;
      fileObj >> vertex.y;
      fileObj >> vertex.z;
      Vertices.push_back({vertex, color});
      std::getline(fileObj, temp);
    }
    // smooth shading (off).
    else if(lineVar[0] == 's')
    {
      std::getline(fileObj, temp);
    }
    // face
    else if(lineVar[0] == 'f')
    {
      unsigned int x, y, z;
      std::getline(fileObj, temp);
      std::stringstream ss(temp);
      if(temp.find("//") != std::string::npos)
      {
        ss >> x;
        ss >> temp;
        ss >> y;
        ss >> temp;
        ss >> z;
        ss >> temp;
      }
      else if (temp.find("/") != std::string::npos)
      {
        ss >> x;
        ss >> temp;
        ss >> y;
        ss >> temp;
        ss >> z;
        ss >> temp;
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
      std::getline(fileObj, temp);
    }
  }
  fileObj.close();
}

void Object::Update(unsigned int dt, int object_num, glm::mat4 planet) // move the object here
{
  // Cleared function for PA4
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