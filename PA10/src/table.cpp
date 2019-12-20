#include "table.h"

Table::Table(char* table_filename){
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError file_loaded = doc.LoadFile(table_filename);
  if(file_loaded != tinyxml2::XML_SUCCESS){
    std::string error;
    std::string filename(table_filename);
    error = "Error loading or parsing master config: " + filename + "\n";
    throw std::logic_error(error);
  }
  tinyxml2::XMLElement* config = doc.FirstChildElement("table-config");
  tinyxml2::XMLElement* element = NULL;

  //set the bumpers
  if((element = config->FirstChildElement("bumpers"))){
    char* filename = new char[256];
    if((element = element->FirstChildElement())){
      strcpy(filename,element->GetText());
      Flipper* flipper = new Flipper(filename);
      objects.push_back(flipper->GetFlipper());
      bumpers.push_back(flipper->GetFlipper());
      while((element = element->NextSiblingElement())){
        strcpy(filename,element->GetText());
        Flipper* new_flipper = new Flipper(filename);
        objects.push_back(new_flipper->GetFlipper());
        bumpers.push_back(flipper->GetFlipper());
      }
    }
    delete filename;
  }



  //set the table object
  if((element = config->FirstChildElement("table"))){
    char* filename = new char[256];
    strcpy(filename,element->GetText());
    table = new Object(filename);
    objects.push_back(table);
    delete filename;
  } 

  //set the left flippers
  if((element = config->FirstChildElement("left-flippers"))){
    char* filename = new char[256];
    if((element = element->FirstChildElement())){
      strcpy(filename,element->GetText());
      Flipper* flipper = new Flipper(filename);
      left_flippers.push_back(flipper);
      objects.push_back(flipper->GetFlipper());
      while((element = element->NextSiblingElement())){
        strcpy(filename,element->GetText());
        Flipper* new_flipper = new Flipper(filename);
        left_flippers.push_back(new_flipper);
        objects.push_back(new_flipper->GetFlipper());
      }
    }
    delete filename;
  }

  //set the right flippers
  if((element = config->FirstChildElement("right-flippers"))){
    char* filename = new char[256];
    if((element = element->FirstChildElement())){
      strcpy(filename,element->GetText());
      Flipper* flipper = new Flipper(filename);
      right_flippers.push_back(flipper);
      objects.push_back(flipper->GetFlipper());
      while((element = element->NextSiblingElement())){
        strcpy(filename,element->GetText());
        Flipper* new_flipper = new Flipper(filename);
        right_flippers.push_back(new_flipper);
        objects.push_back(new_flipper->GetFlipper());
      }
    }
    delete filename;
  }
  //set the ball
  if((element = config->FirstChildElement("ball"))){
    char* filename = new char[256];
    strcpy(filename,element->GetText());
    ball = new Object(filename);
    objects.push_back(ball);
    delete filename;
  }
  starting_ball_position = ball->GetLocation();
}

void Table::FlipLeftFlippers(){
  for(unsigned int i = 0; i < left_flippers.size(); i++){
    left_flippers[i]->Flip(true);
  }
}

void Table::FlipRightFlippers(){
  for(unsigned int i = 0; i < right_flippers.size(); i++){
    right_flippers[i]->Flip(false);
  }
}

void Table::Update(unsigned int dt){
  for(int i = 0; i < left_flippers.size(); i++){
    left_flippers[i]->Update(dt, true);
  }
  for(int i = 0; i < right_flippers.size(); i++){
    right_flippers[i]->Update(dt, false);
  }
}


std::vector<Object*> Table::GetObjects(){
  return objects;
}


std::vector<Object*> Table::GetBumpers(){
  return bumpers;
}

Object* Table::GetBall(){
  return ball;
}

glm::vec3 Table::GetStartingBallPosition(){
  return starting_ball_position;
}
