
//#include "engine.h"
// "engine.h" was calling PA1's engine.h
#include "../../PA2/include/engine.h"


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

bool Engine::Initialize(assets asset)
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
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, asset))
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
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard and mouse input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
      Mouse();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  // handles key down events for PA2
  else if (m_event.type == SDL_KEYDOWN)
  {
    switch(m_event.key.keysym.sym)
    {
      case SDLK_ESCAPE:
        m_running = false;
        break;
      case SDLK_q:
        m_graphics->Input('q');
        break;
      case SDLK_w:
        m_graphics->Input('w');
        break;
      case SDLK_e:
        m_graphics->Input('e');
        break;
      case SDLK_r:
        m_graphics->Input('r');  
        break;
      case SDLK_a:
        m_graphics->Input('a');
        break;
      case SDLK_s:
        m_graphics->Input('s');
        break;
      case SDLK_d:
        m_graphics->Input('d');
        break;
      case SDLK_f:
        m_graphics->Input('f');
        break;
      default:
        break;
    }
  }
}

void Engine::Mouse()
{
  // PA2 mouse inputs
  // Looks for any mouse input down
  if(m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    // Looks for specific mouse click
    switch(m_event.button.button)
    {
      case SDL_BUTTON_LEFT:
        m_graphics->Input('1');
        break;
      case SDL_BUTTON_RIGHT:
        m_graphics->Input('2');
        break;
      case SDL_BUTTON_MIDDLE:
        m_graphics->Input('3');
        break;
      default:
        break;
    }
  }
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
