#include "score.h"

std::vector<Score> ReadScoreFile(char* filename){
	tinyxml2::XMLDocument score_file;
	tinyxml2::XMLError file_loaded = score_file.LoadFile(filename);
	if(file_loaded == tinyxml2::XML_ERROR_FILE_NOT_FOUND){
		std::string error;
		std::string fname(filename);
		error = "XML Error: " + std::to_string(file_loaded) + " in file: "+ fname + "\n";
		throw std::logic_error(error);
	}
	tinyxml2::XMLElement* object = score_file.FirstChildElement("scoreboard");
	tinyxml2::XMLElement* element = object->FirstChildElement();
	tinyxml2::XMLElement* score = element;
	int temp_level;
	float temp_time;
	std::string temp_name;
	std::vector<Score> scores;
	if(element){
		do{
			score = element->FirstChildElement("level");
			temp_level = score->Int64Text();
			score = element->FirstChildElement("time");
			temp_time = score->FloatText();
			scores.push_back(Score(temp_level,temp_time));
			element = element->NextSiblingElement();
		}while(element != NULL);
	}
	return scores;
}

void WriteScoreFile(char* filename, std::vector<Score> scores){
	std::ofstream file;
	file.open(filename,std::ofstream::trunc);
	std::string  element_text;
	file << "<scoreboard>\n";
	for(unsigned int i = 0; i < scores.size(); i++){
		element_text = "s" + std::to_string(i);
		file << "<" << element_text << ">\n";
		file << "<level>" << scores[i].level << "</level>\n";
		file << "<time>" << scores[i].time << "</time>\n";
		file << "</" << element_text << ">\n";
	}
	file << "</scoreboard>\n";
}

bool AddScore(std::vector<Score>& scores, Score new_score){
	bool score_added = false;
	std::vector<Score> new_scores;
	for(unsigned int i = 0; i < scores.size(); i++){		
		if(new_score.time > scores[i].time && scores[i].level == new_score.level){
			score_added  = true;
			new_scores.push_back(new_score);
			new_score = scores[i];
			scores.erase(scores.begin() + i);
			i=0;
		}
	}
	for(unsigned int i = 0; i < new_scores.size(); i++){
		scores.push_back(new_scores[i]);
	}
	return score_added;
}
