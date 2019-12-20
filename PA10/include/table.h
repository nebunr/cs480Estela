#ifndef TABLE_H
#define TABLE_H

#include "object.h"
#include "flipper.h"

class Table{
  public:
    Table(char* table_filename);
    void FlipLeftFlippers();
    void FlipRightFlippers();
    glm::vec3 GetStartingBallPosition();
    Object* GetBall();
    std::vector<Object*> GetObjects();
    std::vector<Object*> GetBumpers();
    void Update(unsigned int dt);
  private:
    std::vector<Flipper*> left_flippers;
    std::vector<Flipper*> right_flippers;
    std::vector<Object*> objects;
    std::vector<Object*> bumpers;
    Object* table;
    Object* ball;
    glm::vec3 starting_ball_position;
};

#endif //TABLE_H
