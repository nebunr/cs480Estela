#include <iostream>
#include <string.h>

#include "engine.h"
#include "assets_headers.h"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  // How to run after make: 
  // ./Tutorial -v vertexfile -f fragmentfile
  // ./Tutorial -v ../assets/vertex_shader.txt -f ../assets/fragment_shader.txt
  assets asset;
  if(strcmp(argv[1], "-v") == 0)
  {
    if(argv[2])
    {
      //../assets/vertex_shader.txt 
      asset.vFile = argv[2];
    }
  }
  if(strcmp(argv[3], "-f") == 0)
  {
    if(argv[4])
    {
      //../assets/fragment_shader.txt
      asset.fFile = argv[4];
    }
  }
  Engine *engine = new Engine("Benjamin Estela - PA1", 800, 600);
  printf("Starting PA1 (ESC to exit)\n");
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
