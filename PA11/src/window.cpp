#include "window.h"

Window::Window()
{
	window = NULL;
}

Window::~Window()
{
	SDL_StopTextInput();
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

bool Window::Initialize(char *name, int *w, int *h)
{
	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL  failed to initialize: %s\n", SDL_GetError());
		return false;
	}

	//Start OpenGL for SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	//Create window
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	//use for fullscreen
	if (*h == 0 && *w == 0)
	{
		*h = current.h;
		*w = current.w;
	}

	window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, *w, *h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window failed to create: %s\n", SDL_GetError());
		return false;
	}

	//Create context
	context = SDL_GL_CreateContext(window);
	if (context == NULL)
	{
		printf("OpenGL context not created: %s\n", SDL_GetError());
		return false;
	}

	//Use VSync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Unable to use VSync: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

void Window::Swap()
{
	SDL_GL_SwapWindow(window);
}

SDL_Window *Window::GetWindow()
{
	return window;
}

SDL_GLContext Window::GetContext()
{
	return context;
}