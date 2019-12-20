#include <iostream>

#include "engine.h"

float Object::orbit_scale;
float Object::planet_scale;
float Object::time_scale;

bool parseCmdArgs(int argc, char **argv, Config* cfg);
void loadConfig(char* config_filename, Config* cfg);

int main(int argc, char **argv)
{
  Config* cfg = new Config;
   
  if(!parseCmdArgs(argc,argv,cfg)){
    return 0;
  }
  Object::orbit_scale = cfg->orbit_scale*2;
  Object::planet_scale = cfg->planet_scale;
  Object::time_scale = cfg->time_scale; 
  // Start an engine and run it then cleanup after
  Engine *engine = new Engine(cfg->window_name, cfg->w, cfg->h);
  if(!engine->Initialize(*cfg))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  printf("Starting Solar System\n");
  printf("Team:\nEstela, Benjamin\nHewitt, Jonathon\nInouye, Sidney\n");
  printf("-----------------\n");
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}

bool parseCmdArgs(int argc, char **argv, Config* cfg){

  char help_message[256];
  sprintf(help_message,"%s -c [config_file]\n",argv[0]);
  if(argc < 2){
    printf("%s",help_message);
    return false;
  }
  for(int i = 1; i < argc; i++){
    if((strcmp(argv[i],"-h")==0)){
      printf("%s",help_message);
      return false;
    }
    else if((strcmp(argv[i],"-c")==0)){
      loadConfig(argv[i+1],cfg);
      return true;
    }
    else{
      printf("Invalid flag, %s \n %s",argv[i],help_message);
      return false;
    }
  }
  return false;
}

void loadConfig(char* config_filename, Config* cfg){
  
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError file_loaded = doc.LoadFile(config_filename);
  if(file_loaded != tinyxml2::XML_SUCCESS){
    std::string error;
    std::string filename(config_filename);
    error = "Error loading or parsing master config: " + filename  + "\n";
    throw std::logic_error(error);
  }
  tinyxml2::XMLElement* config = doc.FirstChildElement("config");
  tinyxml2::XMLElement* element = NULL;

  //set the vertex shader filename
  if((element = config->FirstChildElement("vshader"))){
    cfg->vshader_filename = new char[256];
    strcpy(cfg->vshader_filename,element->GetText());
  }
  
  //set the fragment shader filename
  if((element = config->FirstChildElement("fshader"))){
    cfg->fshader_filename = new char[256];
    strcpy(cfg->fshader_filename,element->GetText());
  }

  //set the window name
  if((element = config->FirstChildElement("windowname"))){
   cfg->window_name = new char[256];
   strcpy(cfg->window_name,element->GetText());
  }

  //set the window width
  if((element = config->FirstChildElement("w"))){
    cfg->w =  element->Int64Text();
  }

  //set the window height
  if((element = config->FirstChildElement("h"))){
    cfg->h = element->Int64Text();
  }

  //set the root object filename
  if((element = config->FirstChildElement("root"))){
    cfg->root_planet_filename = new char[256];
    strcpy(cfg->root_planet_filename,element->GetText());
  }

  //set the global orbit_scale
  if((element = config->FirstChildElement("oscale"))){
    cfg->orbit_scale  = element->FloatText();
  }

  //set the global planet_scale
  if((element = config->FirstChildElement("pscale"))){
    cfg->planet_scale = element->FloatText();
  }

  //set the global time_scale
  if((element = config->FirstChildElement("tscale"))){
    cfg->time_scale = element->FloatText();
  }

}

