#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"
#include "audio.h"
#include "ui.h"

class Engine{
	public:
		Engine(char* name,int w, int h);
		Engine(char* name);
		~Engine();
		bool Initialize(Config cfg);
		void Run();
		bool Keyboard();
		bool Mouse();
		glm::vec2 MouseMovement();
		unsigned int GetDT();
		long long GetCurrentTimeMillis();
	private:
		//Window related variables
		Window* window;
		char* window_name;
		int window_width;
		int window_height;
		bool is_fullscreen;
		SDL_Event event;
		KeyHandler* key_handler;

		Graphics* graphics;
		unsigned int DT;
		long long current_time_millis;
		bool is_running;

		bool died;
		bool win;
		bool quit;
		bool reset;

		Audio* audio;
		Ui* ui;
};

#endif //ENGINE_H
