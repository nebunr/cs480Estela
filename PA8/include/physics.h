#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <vector>
#include "object.h"

class  PhysicsWorld{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		void AddObject(Object*);
		void StepSimulation(unsigned int);

		btDiscreteDynamicsWorld* GetWorld();
		
		void setGravity();

	private:
		btDiscreteDynamicsWorld* dynamics_world;
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collision_config;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
};

#endif
