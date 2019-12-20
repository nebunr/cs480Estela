#include <iostream>
#include "engine.h"

bool parseCmdArgs(int argc, char **argv, Config* cfg);
void loadConfig(char* config_filename, Config* cfg);

int main(int argc, char **argv){
	Config* cfg = new Config;

	if(!parseCmdArgs(argc,argv,cfg)){
    	return 0;
  	}

	Engine* engine = new Engine(cfg->window_name, cfg->w, cfg->h);
	if(!engine->Initialize(*cfg)){
		printf("The engine failed to start.\n");
		delete engine;
		engine = NULL;
		return 1;
	}
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

void loadConfig(char* config_filename, Config* cfg)
{
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

	//set the window
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

	//set up ambient lighting
	if((element = config->FirstChildElement("ambient-r"))){
		cfg->ar = element->FloatText();
	}
	if((element = config->FirstChildElement("ambient-g"))){
		cfg->ag = element->FloatText();
	}
	if((element = config->FirstChildElement("ambient-b"))){
		cfg->ab = element->FloatText();
	}
	if((element = config->FirstChildElement("ambient-intensity"))){
		cfg->ambient_intensity = element->FloatText();
	}
	if((element = config->FirstChildElement("specular-intensity"))){
		cfg->specular_intensity = element->FloatText();
	}

	//set up spotlight lighting
	if((element = config->FirstChildElement("spotlight-r"))){
		cfg->sr = element->FloatText();
	}
	if((element = config->FirstChildElement("spotlight-g"))){
		cfg->sg = element->FloatText();
	}
	if((element = config->FirstChildElement("spotlight-b"))){
		cfg->sb = element->FloatText();
	}
	if((element = config->FirstChildElement("spotlight-location-x"))){
		cfg->sx = element->FloatText();
	}
	if((element = config->FirstChildElement("spotlight-location-y"))){
		cfg->sy = element->FloatText();
	}
	if((element = config->FirstChildElement("spotlight-location-z"))){
		cfg->sz = element->FloatText();
	}
	if((element = config->FirstChildElement("spotlight-cutoff"))){
		cfg->spotlightCutoff = element->FloatText();
  	}
        if((element = config->FirstChildElement("scoreboard"))){
		cfg->scoreboard_filename = new char[256];
		strcpy(cfg->scoreboard_filename, element->GetText());
	}	
}
