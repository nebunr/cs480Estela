
#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(Config cfg)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }
  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, cfg))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
  glm::vec2 mouseLocation = {0.0,0.0};
  char input = '\0';  
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    mouseLocation = MouseMovement();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      input = Keyboard() | Mouse();
      if(input) break;
    }

    // Update and render the graphics
    m_graphics->Update(m_DT,input,mouseLocation);
    m_graphics->Render();
    input = '\0';

    // Swap to the Window
    m_window->Swap();
  }
}

glm::vec2 Engine::MouseMovement()
{
  int x = 0;
  int y = 0;
  SDL_GetRelativeMouseState(&x, &y);
  return glm::vec2(x,y);
}

//gets the mouse event and returns a char represent the mouse action 
char Engine::Mouse()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
    return '\0';
  }
  else if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    switch(m_event.button.button){
      case SDL_BUTTON_LEFT:
        return 1;
        break;
      case SDL_BUTTON_RIGHT:
        return 2;
        break;
      case SDL_BUTTON_MIDDLE:
        return 3;
        break;
      default:
        return '\0';
        break;
    }
  }
  return '\0';
}

//gets the keyboard event and returns a char representing the key hit
char Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
    return '\0';
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    switch(m_event.key.keysym.sym){
      case SDLK_ESCAPE:
        printf("Exiting Solar System\n");
        m_running = false;
        break;
      case SDLK_TAB:
        return '\t';
        break;
      case SDLK_q:
        return 'q';
        break;
      case SDLK_w:
        return 'w';
        break;
      case SDLK_e:
        return 'e';
        break;
      case SDLK_r:
        return 'r';
        break;
      case SDLK_t:
        return 't';
        break;
      case SDLK_y:
        return 'y';
        break;
      case SDLK_a:
        return 'a';
        break;
      case SDLK_s:
        return 's';
        break;
      case SDLK_d:
        return 'd';
        break;
      case SDLK_f:
        return 'f';
        break;
      case SDLK_z:
        return 'z';
        break;
      case SDLK_x:
        return 'x';
        break;
      case SDLK_c:
        return 'c';
        break;
      case SDLK_v:
        return 'v';
        break;
      case SDLK_b:
        return 'b';
        break;
      case SDLK_n:
        return 'n';
        break;
      case SDLK_m:
        return 'm';
        break;
      case SDLK_UP:
        return '^';
        break;
      case SDLK_DOWN:
        return 'V'; // yes, that's a capital v
        break;
      case SDLK_LEFT:
        return '<';
        break;
      case SDLK_RIGHT:
        return '>';
        break;
      default:
        return '\0';
        break;
      }
  }
  return '\0';
}
unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
