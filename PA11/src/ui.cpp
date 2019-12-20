#include "ui.h"

Ui::Ui(SDL_Window *window, SDL_GLContext context)
{
    // Set up context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Initialize opengl and sdl2
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Set up states
    showMainMenu = true;
    showPauseMenu = false;
    showStatistics = false;
    showDeathScreen = false;
    showTimeOut = false;
    showConfirmQuit = false;
    showWin = false;
    showHighScore = false;
    showHighScoreRead = false;

    // Set up scoring
    time = 0;
    score = 0;
    lives = 3;
    level = 0;
}

Ui::~Ui()
{
    // Clean UI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

bool Ui::Initialize()
{
    return true;
}

void Ui::Update(KeyHandler* key_handler)
{
    // Pause game
    if(key_handler->IsPressed('p'))
    {
        if(!showPauseMenu)
        {
            showPauseMenu = true;
            key_handler->Unpress('p');
            return;
        }
        else
        {
            showPauseMenu = false;
            key_handler->Unpress('p');
            return;
        }
    }

    if(key_handler->IsPressed('q'))
    {
        showConfirmQuit = true;
        showPauseMenu = false;
        key_handler->Unpress('q');
    }
}

void Ui::Render(SDL_Window* window, unsigned int dt, bool died, bool win, bool &quit, bool &reset)
{
    // io
    ImGuiIO& io = ImGui::GetIO(); (void) io;

    // New frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    if(showMainMenu)
    {
        // Reset up states
        showPauseMenu = false;
        showStatistics = false;
        showDeathScreen = false;
        showTimeOut = false;
        showHighScoreRead = false;
        showHighScore = false;

        ImGui::SetNextWindowPos( ImVec2( io.DisplaySize.x/2, io.DisplaySize.y/2 ), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize( ImVec2(200, 150));

        // Title bar
        ImGui::Begin("Main Menu");
        
        // Start game
        if(ImGui::Button("Start Game"))
        {
            showMainMenu = false;
            showStatistics = true;
            showPauseMenu = false;
            showDeathScreen = false;
            showTimeOut = false;
            showHighScore = false;
            showHighScoreRead = false;

            // Reset stats
            level = 0;
            time = 60;
            score = 0;
            lives = 3;
        }

        // Choose level
        ImGui::Text("Choose Level");
        if(ImGui::Button("Level 1"))
        {
            level = 0;
            time = 60;
            score = 0;
            lives = 3;

            showMainMenu = false;
            showStatistics = true;
            showPauseMenu = false;
            showDeathScreen = false;
            showTimeOut = false;
            reset = true;
            showHighScore = false;
            showHighScoreRead = false;
        }
        ImGui::SameLine();
        if(ImGui::Button("Level 2"))
        {
            level = 1;
            time = 60;
            score = 0;
            lives = 3;
            
            showMainMenu = false;
            showStatistics = true;
            showPauseMenu = false;
            showDeathScreen = false;
            showTimeOut = false;
            showHighScore = false;
            showHighScoreRead = false;
            reset = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("Level 3"))
        {
            level = 2;
            time = 60;
            score = 0;
            lives = 3;
            
            showMainMenu = false;
            showStatistics = true;
            showPauseMenu = false;
            showDeathScreen = false;
            showTimeOut = false;
            showHighScore = false;
            showHighScoreRead = false;
            reset = true;
        }

        // Show high score
        if(ImGui::Button("Show High Scores"))
        {
            showHighScoreRead = true;
            showMainMenu = false;
        }
        
        // Quit Game
        if(ImGui::Button("Quit Game"))
        {
            quit = true;
            printf("Exiting Program\n");
        }

        ImGui::End();
    }

    if(showConfirmQuit)
    {
        ImGui::SetNextWindowPos(ImVec2( io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f ), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        ImGui::Begin("Confirm Exit");

        ImGui::Text("Are you sure you want to return to main menu?");
        if(ImGui::Button("Yes, Quit"))
        {
            showConfirmQuit = false;
            showStatistics = false;
            showPauseMenu = false;
            showMainMenu = true;
            showHighScore = false;
            showHighScoreRead = false;
            reset = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("No, Don't Quit"))
        {
            showConfirmQuit = false;
        }

        ImGui::End();
    }

    // Score bar
    if(showStatistics)
    {
        ImGui::SetNextWindowPos(ImVec2( io.DisplaySize.x * 0.1f, io.DisplaySize.y * 0.15f ), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        // Title bar
        ImGui::Begin("ApeSphere");
        
        // Time
        ImGui::Text("Time: %.1f", time);
        if(!showPauseMenu && !showDeathScreen && !showConfirmQuit && !showTimeOut && !showWin && !showHighScore)
            time -= dt/1000.f;

        // Lives
        if(died)
            lives--;
        
        if(win)
        {
            Score newScore(level, time);

            if(AddScore(scores, newScore))
            {
                WriteScoreFile(scoreboard_filename, scores);
                showHighScore = true;
            }
            else
            {
               showWin = true;
            }
        }


        ImGui::Text("Lives: %d", lives);

        // Game over if statements
        if(time <= 0)
        {
            showTimeOut = true;
            time = 0;
        }

        if(lives == 0)
        {
            showDeathScreen = true;
        }

        // Pause button
        if( ImGui::Button("Pause Menu") )
        {
            if(showPauseMenu)
                showPauseMenu = false;
            else
                showPauseMenu = true;
        }
        ImGui::End();
    }

    if(showPauseMenu)
    {
        if(!showDeathScreen & !showMainMenu)
        {
            ImGui::SetNextWindowPos(ImVec2( io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f ), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize( ImVec2(210, 125));
 
            ImGui::Begin("Pause");

            //Return to game
            if(ImGui::Button("Return to Game"))
            {
                showPauseMenu = false;
            }

            // Choose level
            ImGui::Text("Choose Level");
            if(ImGui::Button("Level 1"))
            {
                level = 0;
                time = 60;
                score = 0;
                lives = 3;

                showMainMenu = false;
                showStatistics = true;
                showPauseMenu = false;
                showDeathScreen = false;
                showTimeOut = false;
                showHighScore = false;
                showHighScoreRead = false;
                reset = true;
            }
            ImGui::SameLine();
            if(ImGui::Button("Level 2"))
            {
                level = 1;
                time = 60;
                score = 0;
                lives = 3;
                
                showMainMenu = false;
                showStatistics = true;
                showPauseMenu = false;
                showDeathScreen = false;
                showTimeOut = false;
                showHighScore = false;
                showHighScoreRead = false;
                reset = true;
            }
        ImGui::SameLine();
        if(ImGui::Button("Level 3"))
        {
            level = 2;
            time = 60;
            score = 0;
            lives = 3;
            
            showMainMenu = false;
            showStatistics = true;
            showPauseMenu = false;
            showDeathScreen = false;
            showTimeOut = false;
            showHighScore = false;
            showHighScoreRead = false;
            reset = true;
        }

            //Open main menu
            if(ImGui::Button("Main Menu"))
            {
                showConfirmQuit = true;
            }

            ImGui::End();
        }
        else
        {
            showPauseMenu = false;
        }
        
    }

    if(showDeathScreen)
    {
        ImGui::SetNextWindowPos(ImVec2( io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f ), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize( ImVec2(135, 100));

        ImGui::Begin("You Died!");

        // Final score
        ImGui::Text("Final score: %.1f", 60 - time);

        // Retry
        if(ImGui::Button("Retry Level"))
        {
            lives = 3;
            time = 60;
            score = 0;
            showDeathScreen = false;
        }

        // Go to main menu
        if(ImGui::Button("Main Menu"))
        {
            showMainMenu = true;
            reset = true;
            showDeathScreen = false;
        }

        ImGui::End();
    }

    if(showTimeOut)
    {
        ImGui::SetNextWindowPos(ImVec2( io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f ), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize( ImVec2(135, 100));

        ImGui::Begin("You Ran Out Of Time!");

        // Final score
        ImGui::Text("Final score: %.1f", 60 - time);

        // Retry
        if(ImGui::Button("Retry Level"))
        {
            lives = 3;
            time = 60;
            score = 0;
            showTimeOut = false;
            reset = true;
        }

        // Go to main menu
        if(ImGui::Button("Main Menu"))
        {
            showMainMenu = true;
            reset = true;
            showTimeOut = false;
        }

        ImGui::End();
    }

    if(showWin)
    {
        ImGui::SetNextWindowPos(ImVec2( io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f ), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize( ImVec2(135, 125));

        ImGui::Begin("You Won!");

        // Final Score
        ImGui::Text("Final Score: %.1f", 60 - time);

        // Retry
        if(ImGui::Button("Retry Level"))
        {
            lives = 3;
            time = 60;
            score = 0;
            showWin = false;
            reset = true;
        }        

        // Next level
        if(ImGui::Button("Next Level"))
        {
            level++;
            time = 60;
            lives = 3;
            score = 0;
            showWin = false;
            reset = true;
            //go back to first level from 2nd one
            if(level > 2)
            {
                level = 0;
            }
        }

        // Main Menu
        if(ImGui::Button("Main Menu"))
        {
            showMainMenu = true;
            reset = true;
            showWin = false;
        }

        ImGui::End();
    }

    // When there is a new high score, show this
    if(showHighScore)
    {
        ImGui::SetNextWindowPos(ImVec2( io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f ), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("New High Score!");

        if(ImGui::Button("Close"))
        {
            showHighScore = false;
            showWin = true;
        }

        ImGui::End();
    }

    if(showHighScoreRead)
    {
        ImGui::SetNextWindowPos(ImVec2( io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f ), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(135, 300));

        ImGui::Begin("High Score List");
        
        // Load levels and sort
        Score levelScores[3][3];
        int j = 0;
        int k = 0;
        int m = 0;
        for(int i = 0; i < 9; i++)
        {
            if(0 == scores[i].level)
            {
                levelScores[0][j] = scores[i];
                j++;
            }
            if(1 == scores[i].level)
            {
                levelScores[1][k] = scores[i];
                k++;
            }
            if(2 == scores[i].level)
            {
                levelScores[2][m] = scores[i];
                m++;
            }
        }
        

        // Show level scores
        for(int i = 0; i < 3; i++)
        {
            ImGui::Text("Level %d", i + 1);
            for(int n = 0; n < 3; n++)
            {
                ImGui::Text("%d) %.1f", n + 1, 60 - levelScores[i][n].time);
            }
        }

        ImGui::Text("\n");

        if(ImGui::Button("Close"))
        {
            showHighScoreRead = false;
            showMainMenu = true;
        }

        ImGui::End();
    }

    // Render menus
    ImGui::Render();
    glViewport(0,0, (int) io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Ui::GetPauseState()
{
    return showPauseMenu;
}

bool Ui::GetDeathState()
{
    return showDeathScreen;
}

bool Ui::GetTimeOutState()
{
    return showTimeOut;
}

bool Ui::GetMainMenuState()
{
    return showMainMenu;
}

bool Ui::GetConfirmState()
{
    return showConfirmQuit;
}

bool Ui::GetWinState()
{
    return showWin;
}

bool Ui::GetHSState()
{
    return showHighScore;
}

bool Ui::GetHSRState()
{
    return showHighScoreRead;
}

int Ui::GetLevel()
{
    return level;
}

void Ui::SetScoreboard(char* filename){
	scoreboard_filename = filename;
	scores = ReadScoreFile(filename);
}
