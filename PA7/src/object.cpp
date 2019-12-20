#include "object.h"

Object::Object()
{
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
  parent = NULL;
}

Object::Object(char* object_config_filename)
{ 
  parseObjectConfig(object_config_filename);
  parent = NULL;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  time_scale = .01;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

//takes a character representing keyboard input and performs the
//proper mutations on the objects config
void Object::processInput(char input){
 switch(input){
    case 'q':
      time_scale -= 0.05;
      break;
    case 'w':
      time_scale += 0.05;
      break;
    case 'e':
      config.rotation_speed += 0.05;
      break;
    case 'r':
      if(config.rotation_paused){
        config.rotation_paused = false;
      }
      else{
        config.rotation_paused = true;
      }
      break;
    case 'a':
      config.orbit_direction *= -1;
      break;
    case 's':
      config.orbit_speed -= 0.5;
      break;
    case 'd':
      config.orbit_speed += 0.5;
      break;
    case 'b':
      if(config.orbit_paused){
        config.orbit_paused = false;
      }
      else{
        config.orbit_paused = true;
      } 
      break;
    case 'z':
      config.orbit_distance /= 1.1;
      break;
    case 'x':
      config.orbit_distance *= 1.1;
      break;
    case 'c':
      config.scale /= 1.1;
      break;
    case 'v':
      config.scale *= 1.1;
      break;
    default:
      break;
  }
}

void Object::Update(unsigned int dt, bool flat_earth)
{
  
  //If rotation/orbit are not paused, increment the angles.
  if(!config.orbit_paused){
    if(config.orbit_direction > 0){
      config.orbit_angle += dt * time_scale * (M_PI + config.orbit_speed)/1000;
    }
    else{
      config.orbit_angle -= dt * time_scale * (M_PI + config.orbit_speed)/1000;
    }
  }
  if(!config.rotation_paused){
    if(config.rotation_direction > 0){
      config.rotation_angle += dt * time_scale * (M_PI + config.rotation_speed)/1000;
    }
    else{
      config.rotation_angle -= dt * time_scale * (M_PI + config.orbit_speed)/1000;
    }
  }

  //Calculate the final angles with all scalars.
  float orbit_angle = config.orbit_angle;
  float rotation_angle = config.rotation_angle;

  glm::mat4 orbit;
  glm::mat4 rotation;
  glm::mat4 scale;
  
  //perform the rotations and translations
  orbit = glm::rotate(glm::mat4(1.0f),orbit_angle,config.orbit_axis); 
  orbit *= glm::translate(glm::mat4(1.0f),glm::vec3(orbit_scale * config.orbit_distance,0.0,0.0));
  orbit *= glm::rotate(glm::mat4(1.0f),-orbit_angle,config.orbit_axis);
  location = orbit;
  //rotation = glm::rotate(glm::mat4(1.0f),rotation_angle,config.rotation_axis);
  //for flat earth
  if(flat_earth == true)
  {
    scale = glm::scale(glm::mat4(1.0f),
            glm::vec3(config.scale*planet_scale, 1.0, config.scale*planet_scale));
    rotation = glm::rotate(glm::mat4(1.0f),0.0f,config.rotation_axis);
  }
  else
  {
    scale = glm::scale(glm::mat4(1.0f),glm::vec3(config.scale*planet_scale));
    rotation = glm::rotate(glm::mat4(1.0f),rotation_angle,config.rotation_axis);
  }
  //scale = glm::scale(glm::mat4(1.0f),glm::vec3(config.scale*planet_scale));
  if(parent != NULL){
    model = parent->GetLocation() * orbit * rotation * scale;
    location = parent->GetLocation() * orbit;
  }
  else{
    model = orbit * rotation * scale;
  }
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glBindTexture(GL_TEXTURE_2D,m_texture);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture_coordinates));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

//Parses planet config files and creates an object with the proper config settings.
//An example config file can be found in ../assets/entities/planet.conf
void Object::parseObjectConfig(char* object_config_filename){
  if(!object_config_filename){
    return;
  }

  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError file_loaded = doc.LoadFile(object_config_filename);
  if(file_loaded != tinyxml2::XML_SUCCESS){
    std::string error;
    std::string filename(object_config_filename);
    error  = "Could not find XML file: " + filename + "\n";
    throw std::logic_error(error);
  }
  tinyxml2::XMLElement* object = doc.FirstChildElement("planet");
  tinyxml2::XMLElement* element = NULL;

  //set the model
  if((element = object->FirstChildElement("model"))){
    char* filename = new char[256];
    strcpy(filename,element->GetText());
    parseObjFile(filename);
    delete filename;
  }

  //set the name
  if((element = object->FirstChildElement("name"))){
    char* name = new char[256];
    strcpy(name,element->GetText());
    config.name = name;
    delete name;
  }

  //set the orbit_speed
  if((element = object->FirstChildElement("ospeed"))){
    config.orbit_speed = element->FloatText();
  }

  //set the orbit_angle
  if((element = object->FirstChildElement("oangle"))){
    config.orbit_angle = element->FloatText();
  }

  //set the orbit_distance
  if((element = object->FirstChildElement("odistance"))){
    config.orbit_distance = element->FloatText();
  }
  
  //set the orbit_axis
  if((element = object->FirstChildElement("oaxis"))){
    float angle_radians = element->FloatText() * M_PI / 180;
    config.orbit_axis = glm::vec3(0.0,cos(angle_radians),sin(angle_radians));
  }

  //set the orbit_paused flag
  if((element = object->FirstChildElement("opaused"))){
    config.orbit_paused = element->BoolText();
  }

  //set the rotation_speed
  if((element = object->FirstChildElement("rspeed"))){
    config.rotation_speed = element->FloatText();
  }

  //set the rotation_angle
  if((element = object->FirstChildElement("rangle"))){
    config.rotation_angle = element->FloatText();
  }

  //set the rotation_acis
  if((element = object->FirstChildElement("raxis"))){
    float angle_radians = (element->FloatText() + 90) * M_PI / 180;
    config.rotation_axis = glm::vec3(cos(angle_radians),sin(angle_radians),0.0);
  }

  //set the rotation_paused flag
  if((element = object->FirstChildElement("rpaused"))){
    config.rotation_paused = element->BoolText();
  }

  //set the scale
  if((element = object->FirstChildElement("scale"))){
    config.scale = element->FloatText();
  }

  //set the texture filepath and load the texture
  if((element = object->FirstChildElement("texture"))){
    strcpy(config.texture_filepath,element->GetText());
    loadTexture(config.texture_filepath);
  }

  //set satelites
  if((element = object->FirstChildElement("satelites"))){
    char* filename = new char[256];
    if((element = element->FirstChildElement())){
      strcpy(filename,element->GetText());
      Object* satelite = new Object(filename);
      satelite->setParent(this);
      satelites.push_back(satelite);
      while((element = element->NextSiblingElement())){
	strcpy(filename,element->GetText());
        Object* new_satelite = new Object(filename);
	new_satelite->setParent(this);
	satelites.push_back(new_satelite);
      }
    }
    delete filename;
  }

}

void Object::parseObjFile(char* obj_filename){
  Assimp::Importer importer;
  const aiScene* my_scene = importer.ReadFile(obj_filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
  if(!my_scene){
    throw std::logic_error("Could not open .obj file");
  } 
  //assuming there is only one mesh, which should be the case for .obj files, the first mesh is
  //the one we're interested in
  for(unsigned int j = 0; j < my_scene->mNumMeshes; j++){ 
  aiMesh* mesh = my_scene->mMeshes[j];
  //Process Vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
      aiVector3D ai_vec = mesh->mVertices[i];
      aiVector3D ai_texture = mesh->mTextureCoords[0][i]; 
      glm::vec3 vertex = {ai_vec.x,ai_vec.y,ai_vec.z};
      glm::vec2 texture_coordinates = {ai_texture.x,ai_texture.y};
      Vertices.push_back({vertex,texture_coordinates});
    }
  
  //Process Faces
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
      aiFace face = mesh->mFaces[i];
    
    //if we were not given triangles throw an error and abort
      if(face.mNumIndices != 3){
        std::string error;
        std::string file_name(obj_filename);
        error = "Expected triangles in faces from file: " + file_name + " but recived " + std::to_string(mesh->mNumFaces) + " indices.\n";
        throw std::logic_error(error);
      }
      Indices.push_back(face.mIndices[0]);
      Indices.push_back(face.mIndices[1]);
      Indices.push_back(face.mIndices[2]);
    }
  }
}

void Object::loadTexture(char* texture_filepath){
  Magick::Image* image;
  Magick::Blob blob;
  GLuint texture;

  Magick::InitializeMagick(NULL);

  image = new Magick::Image(texture_filepath);
  image->write(&blob,"RGBA");
  glGenTextures(1,&texture);
  setTexture(texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,texture);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image->columns(),image->rows(),-0.5,GL_RGBA,GL_UNSIGNED_BYTE,blob.data());
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D,0);
}

GLuint Object::getTexture(){
	return m_texture;
}

void Object::setTexture(GLuint text){
	m_texture = text;
}

std::vector<Object*> Object::getSatelites(){
  return satelites;
}

void Object::setSatelites(std::vector<Object*> s){
  satelites = s;
}

Object* Object::getParent(){
  return parent;
}

void Object::setParent(Object* p){
  parent = p;
}

glm::mat4 Object::GetLocation(){
  return location;
}

bool Object::isSelected(){
  return selected;
}
void Object::Select(){
  selected = true;
}
void Object::Deselect(){
  selected = false;
}

std::string Object::getName(){
  return config.name;
}
float Object::getScale(){
  return config.scale;
}
