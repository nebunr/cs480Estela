#include "engine.h"

Engine::Engine(char* name, int w, int h){
	window_name = name;
	window_width = w;
	window_height = h;
	is_fullscreen = false;
}

Engine::Engine(char* name){
	window_name = name;
	window_width = 0;
	window_height = 0;
	is_fullscreen = true;
}

Engine::~Engine(){
	delete window;
	delete graphics;
	window = NULL;
	graphics = NULL;
}

bool Engine::Initialize(Config cfg){
	//Start a window
	window = new Window();
	if(!window->Initialize(window_name,&window_width,&window_height)){
		printf("The window failed to initialize.\n");
		return false;
	}
	//Start the graphics
	graphics = new Graphics();
	if(!graphics->Initialize(window_width,window_height,cfg)){
		printf("The graphics failed to initialize.\n");
		return false;
	}

	//Start the audio
	audio = new Audio();
	if(!audio->Initialize()){
		printf("The audio failed to intialize.\n");
		return false;
	}

	//Start the UI
	ui = new Ui(window->GetWindow(), window->GetContext());
	if(!ui->Initialize()) {
		printf("UI failed to initialize\n");
		return false;
	}
	if(cfg.scoreboard_filename){
		ui->SetScoreboard(cfg.scoreboard_filename);
	}

	//Set the time
	current_time_millis = GetCurrentTimeMillis();
	//Set up the keyhandler
	key_handler = new KeyHandler();
	return true;
}

void Engine::Run(){
	bool input = false;
	is_running = true;
	died = false;
	int level = 0;
	win = false;
	quit = false;
	while(is_running){
		//Update the DT
		DT = GetDT();
		//Check the keyboard input
		while(SDL_PollEvent(&event) != 0){
			input = Keyboard() | Mouse();
			if(input) break;
		}
		//Update and render the graphics
		//Do this only if the game is playing
		if(!ui->GetDeathState() && !ui->GetPauseState() && !ui->GetMainMenuState() && !ui->GetConfirmState() && !ui->GetTimeOutState() && !ui->GetWinState() && !ui->GetHSState() && !ui->GetHSRState())
		{
			graphics->Update(DT,key_handler, died, win, level);
			graphics->Render();
		}

		//Check audio (comment out to disable)
		audio->Update(ui->GetPauseState() || ui->GetDeathState() || ui->GetTimeOutState());

		//Update and render the ui
		ui->Update(key_handler);
		ui->Render(window->GetWindow(), DT, died, win, quit, reset);
		level = ui->GetLevel();

		// Reset if main menu is called
		if(reset)
		{
			graphics->Reset(level);
		}

		//Reset all boolean
		died = false;
		win = false;
		reset = false;

		//Swap to the Window
		window->Swap();

		if(quit)
		{
			is_running = false;
		}
	}
}

bool Engine::Mouse(){
	if(event.type == SDL_QUIT){
		is_running = false;
		return false;
	}
	else if(event.type == SDL_MOUSEBUTTONDOWN){
		switch(event.button.button){
			case SDL_BUTTON_LEFT:
				key_handler->Press('1');
				return true;
				break;
			case SDL_BUTTON_RIGHT:
				key_handler->Press('2');
				return true;
				break;
			case SDL_BUTTON_MIDDLE:
				key_handler->Press('3');
				return true;
				break;
			default:
				return false;
				break;
			}
	}
	else if(event.type == SDL_MOUSEBUTTONUP){
		switch(event.button.button){
			case SDL_BUTTON_LEFT:
				key_handler->Unpress('1');
				return true;
				break;
			case SDL_BUTTON_RIGHT:
				key_handler->Unpress('2');
				return true;
				break;
			case SDL_BUTTON_MIDDLE:
				key_handler->Press('3');
				return true;
				break;
			default:
				return false;
				break;
		}
	}
	return false;
}

bool Engine::Keyboard(){
	if(event.type == SDL_QUIT){
		is_running = false;
		return '\0';
	}
	else if(event.type == SDL_KEYDOWN){
		switch(event.key.keysym.sym){
			case SDLK_ESCAPE:
				printf("Exiting Program\n");
				is_running = false;
				return false;
				break;
			case SDLK_w:
				key_handler->Press('w');
				return true;
				break;
			case SDLK_a:
				key_handler->Press('a');
				return true;
				break;
			case SDLK_s:
				key_handler->Press('s');
				return true;
				break;
			case SDLK_d:
				key_handler->Press('d');
				return true;
				break;
			default:
				return true;
				break;
		}
	}
	else if(event.type == SDL_KEYUP){
		switch(event.key.keysym.sym){
			case SDLK_ESCAPE:
				printf("Exiting Program\n");
				is_running = false;
				return false;
				break;
			case SDLK_w:
				key_handler->Unpress('w');
				return true;
				break;
			case SDLK_a:
				key_handler->Unpress('a');
				return true;
				break;
			case SDLK_s:
				key_handler->Unpress('s');
				return true;
				break;
			case SDLK_d:
				key_handler->Unpress('d');
				return true;
				break;
			case SDLK_p:
				key_handler->Press('p');
				return true;
				break;
			case SDLK_q:
				key_handler->Press('q');
				return true;
				break;
			default:
				return true;
				break;
		}
	}
	return false;
}

unsigned int Engine::GetDT(){
	long long TimeNowMillis = GetCurrentTimeMillis();
	assert(TimeNowMillis >= current_time_millis);
	unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - current_time_millis);
	current_time_millis = TimeNowMillis;
	return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis(){
	timeval t;
	gettimeofday(&t, NULL);
	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
}
