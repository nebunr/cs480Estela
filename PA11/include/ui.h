#ifndef UI_H_
#define UI_H_

#include "imgui.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "graphics_headers.h"
#include "keyhandler.h"
#include "score.h"

class Ui
{
    public:
        Ui(SDL_Window* window, SDL_GLContext context);
        ~Ui();
        bool Initialize();
        void Update(KeyHandler* key_handler);
        void Render(SDL_Window* window, unsigned int dt, bool died, bool win, bool& quit, bool &reset);
        
        bool GetPauseState();
        bool GetDeathState();
        bool GetMainMenuState();
        bool GetConfirmState();
        bool GetTimeOutState();
        bool GetWinState();
        bool GetHSState();
        bool GetHSRState();
        int GetLevel();
	void SetScoreboard(char* filename);

    private:
        SDL_GLContext gl_context;
        
        // States of windows
        bool showMainMenu;
        bool showTimeOut;
        bool showPauseMenu;
        bool showStatistics;
        bool showDeathScreen;
        bool showConfirmQuit;
        bool showWin;
        bool showHighScoreRead;
        bool showHighScore;

	    std::vector<Score> scores;

        float time;
        int score;
        int lives;
        int level;
	    char* scoreboard_filename;
        std::string name;
};

#endif
