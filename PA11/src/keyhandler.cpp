#include "keyhandler.h"

KeyHandler::KeyHandler(){
	for(int i = 0; i < 256; i++){
		keys[i] = false;
	}
}

KeyHandler::~KeyHandler(){}

void KeyHandler::Press(char key){
	keys[key] = true;
}

void KeyHandler::Unpress(char key){
	keys[key] = false;
}

bool KeyHandler::IsPressed(char key){
	return keys[key];
}
