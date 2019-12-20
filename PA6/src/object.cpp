#include "object.h"

Object::Object()
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
  */
}

Object::Object(char* object_config_filename,char* obj_filename,float scl)
{  
  parseObjFile(obj_filename);

  for(int i = 0; i < numMeshes; i++)
  {
    glGenBuffers(1, &VB[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VB[i]);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices[i].size(), &Vertices[i][0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB[i]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices[i].size(), &Indices[i][0], GL_STATIC_DRAW);
  }
  //Parse an object config file and makes the appropriate assignments to the config
  config = parseObjectConfig(object_config_filename);
  if(scl > 0.0){
    config.scale = scl;
  }
}

Object::~Object()
{
  for(int i = 0; i < numMeshes; i++)
  {
    Vertices[i].clear();
    Indices[i].clear();
  }
}

//takes a character representing keyboard input and performs the
//proper mutations on the objects config
void Object::processInput(char input){
 switch(input){
    case 'q':
      config.rotation_direction *= -1;
      break;
    case 'w':
      config.rotation_speed -= 0.5;
      break;
    case 'e':
      config.rotation_speed += 0.5;
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
    case 'f':
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

void Object::Update(unsigned int dt)
{
  model = glm::scale(glm::mat4(1.0f),glm::vec3(config.scale));
  /*
  //If rotation/orbit are not paused, increment the angles.
  if(!config.orbit_paused){
    if(config.orbit_direction > 0){
      config.orbit_angle += dt * (M_PI + config.orbit_speed)/1000;
    }
    else{
      config.orbit_angle -= dt * (M_PI + config.orbit_speed)/1000;
    }
  }
  if(!config.rotation_paused){
    if(config.rotation_direction > 0){
      config.rotation_angle += dt * (M_PI + config.rotation_speed)/1000;
    }
    else{
      config.rotation_angle -= dt * (M_PI + config.orbit_speed)/1000;
    }
  }
  //Calculate the final angles with all scalars.
  float orbit_angle = config.orbit_angle;
  float rotation_angle = config.rotation_angle;

  glm::mat4 orbit;
  glm::mat4 rotation;
  glm::mat4 scale;
  
  //perform the rotations and translations
  orbit = glm::rotate(glm::mat4(1.0f),orbit_angle,glm::vec3(0.0,1.0,0.0)); 
  orbit *= glm::translate(glm::mat4(1.0f),glm::vec3(config.orbit_distance,0.0,0.0));
  orbit *= glm::rotate(glm::mat4(1.0f),-orbit_angle,glm::vec3(0.0,1.0,0.));
  location = orbit;
  rotation = glm::rotate(glm::mat4(1.0f),rotation_angle,glm::vec3(0.0,1.0,0.0));
  scale = glm::scale(glm::mat4(1.0f),glm::vec3(config.scale));
  if(parent != NULL){
    model = parent->GetLocation() * orbit * rotation * scale;
  }
  else{
    model = orbit * rotation * scale;
  }*/
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  for( int i = 0; i < numMeshes; i++ )
  {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, VB[i]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture_coordinates));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);

    //bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture[i]);

    glDrawElements(GL_TRIANGLES, Indices[i].size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }
}

//Parses planet config files and creates an object with the proper config settings.
//An example config file can be found in ../assets/entities/planet.conf
ObjectConfig Object::parseObjectConfig(char* object_config_filename)
{
  std::string temp_string;
  std::ifstream config_file;
  config_file.open(object_config_filename);
  if(!config_file.good()){
    printf("Invalid object config filepath: %s\nUsing default object config.\n",object_config_filename);
    return ObjectConfig();
  }
}

void Object::parseObjFile(char* obj_filename){
  //set up importer and load in my_scene
  Assimp::Importer importer;
  const aiScene* my_scene = importer.ReadFile(obj_filename, aiProcess_Triangulate);

  //variables for getting texture info and storing imagedata
  std::string texture_filepath;
  aiString aistring_filename;
  GLuint texture;
  Magick::Image* image;
  Magick::Blob blob;

  //Initialize Magick
  Magick::InitializeMagick(NULL);

  numMeshes = my_scene->mNumMeshes;

  //iterate through meshes and process their vertices,faces and texture bindings
  //unable to make j into a unsigned int at the moment for PA6
  for( int j = my_scene->mNumMeshes - 1; j >= 0; j-- )
  {
    //set mesh to current mesh to process
    aiMesh* mesh = my_scene->mMeshes[j];

    // if object has a mtl file, process images
    if( my_scene->HasMaterials() )
    {
      //load the material, and get the path for the texture image
      const aiMaterial* material = my_scene->mMaterials[j + 1];
      material->GetTexture(aiTextureType_DIFFUSE,0,&aistring_filename,NULL,NULL,NULL,NULL,NULL);
      
      //prepend ../assets/textures/ to the image filename so it knows where to look
      texture_filepath = "../assets/textures/" + std::string(aistring_filename.C_Str());

      //load the image and write it into blob
      image = new Magick::Image(texture_filepath);
      image->write(&blob,"RGBA");

      //Bind Textures
      glGenTextures(1,&texture);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image->columns(),image->rows(),-0.5,GL_RGBA,GL_UNSIGNED_BYTE,blob.data());
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

      m_texture[j] = texture;
    }

    //Process Vertices
    for(int i = 0; i < mesh->mNumVertices; i++){
      aiVector3D ai_vec = mesh->mVertices[i]; 
      aiVector3D ai_texture = mesh->mTextureCoords[0][i];
      glm::vec3 vertex = {ai_vec.x,ai_vec.y,ai_vec.z};
      glm::vec2 texture_coordinates = {ai_texture.x,ai_texture.y};
      Vertices[j].push_back({vertex,texture_coordinates});
    }
  
    //Process Faces
    for(int i = 0; i < mesh->mNumFaces; i++){
      aiFace face = mesh->mFaces[i];

      //if we were not given triangles throw an error and abort
      if(face.mNumIndices != 3){
        std::string error;
        std::string file_name(obj_filename);
        error = "Expected triangles in faces from file: " + file_name + " but received " + std::to_string(mesh->mNumFaces) + " indices.\n";
        throw std::logic_error(error);
      }
      Indices[j].push_back(face.mIndices[0]);
      Indices[j].push_back(face.mIndices[1]);
      Indices[j].push_back(face.mIndices[2]);
    }
  }
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

void Object::setScale(float s){
  config.scale = s;
}
