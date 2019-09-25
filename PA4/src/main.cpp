#include <iostream>
#include <string.h>

#include "engine.h"
#include "assets_headers.h"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  // How to run after make: 
  // ./Tutorial -f fragmentfile -v vertexfile -b objectfile
  // ./Tutorial -f ../assets/fragment_shader.txt -v ../assets/vertex_shader.txt -b ../assets/tray.obj
  assets asset;
  for (int i = 1; i < argc; i++)
  {
    if(strcmp(argv[i], "-v") == 0)
    {
      if(argv[i+1])
      {
        asset.vFile = argv[i+1];
      }
    }
    else if(strcmp(argv[i], "-f") == 0)
    {
      if(argv[i+1])
      {
        asset.fFile = argv[i+1];
      }
    }
    else if(strcmp(argv[i], "-b") == 0)
    {
      if(argv[i+1])
      {
        asset.bFile = argv[i+1];
      }
    }
  }
  Engine *engine = new Engine("Benjamin Estela - PA4", 800, 600);
  printf("Starting.\n");
  if(!engine->Initialize(asset))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
