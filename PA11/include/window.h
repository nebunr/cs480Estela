#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window{
	public:
		Window();
		~Window();
		bool Initialize(char* name, int* width, int* height);
		void Swap();
		SDL_Window* GetWindow();
		SDL_GLContext GetContext();
	private:
		SDL_Window* window;
		SDL_GLContext context;
};

#endif //WINDOW_H
