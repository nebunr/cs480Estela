#include "flipper.h"

Flipper::Flipper(char* flipper_filename){
  flipper_object = new Object(flipper_filename);
}

void Flipper::Flip(bool left){
  if(left)
  {
    yaw = 1.f;
    btQuaternion turn(yaw, 0,0);
    btTransform trans;
    flipper_object->GetRigidBody()->getMotionState()->getWorldTransform(trans);
    trans.setRotation(turn);
    flipper_object->GetRigidBody()->getMotionState()->setWorldTransform(trans);
  }
  else
  {
    yaw = 2 * M_PI - 1.f;
    btQuaternion turn(-yaw, 0,0);
    btTransform trans;
    flipper_object->GetRigidBody()->getMotionState()->getWorldTransform(trans);
    trans.setRotation(turn);
    flipper_object->GetRigidBody()->getMotionState()->setWorldTransform(trans);
  }
  
}

void Flipper::Update(unsigned int dt, bool left){
  if(left)
  {
    yaw -= 0.3 * dt/100;
    if(yaw <= 0)
      yaw = 0;
    btQuaternion turn(yaw, 0,0);
    btTransform trans;
    flipper_object->GetRigidBody()->getMotionState()->getWorldTransform(trans);
    trans.setRotation(turn);
    flipper_object->GetRigidBody()->getMotionState()->setWorldTransform(trans);
  }
  else
  {
    yaw += 0.3 * dt/100;
    if(yaw >= 2 * M_PI)
      yaw = 2 * M_PI;
    btQuaternion turn(yaw, 0,0);
    btTransform trans;
    flipper_object->GetRigidBody()->getMotionState()->getWorldTransform(trans);
    trans.setRotation(turn);
    flipper_object->GetRigidBody()->getMotionState()->setWorldTransform(trans);
  }
}
Object* Flipper::GetFlipper(){
  return flipper_object;
}
