#ifndef FLIPPER_H
#define FLIPPER_H

#include "object.h"

class Flipper{
  public:
    Flipper(char* flipper_filename);
    void Flip(bool left);
    Object* GetFlipper();
    void Update(unsigned int dt, bool left);
  private:
    Object* flipper_object;
    float yaw;
};

#endif //FLIPPER_H
