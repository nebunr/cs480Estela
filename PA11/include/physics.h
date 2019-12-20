#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class PhysicsWorld{
	public:
		PhysicsWorld();
		PhysicsWorld(btVector3 gravity);
		~PhysicsWorld();
		void SetGravity(btVector3 gravity);
		btVector3 GetGravity();
		void AddBody(btRigidBody* bd);
		void StepSimulation(unsigned int dt);
	private:
		btDiscreteDynamicsWorld* dynamics_world;
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collision_config;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
};

#endif //PHYSICS_H
