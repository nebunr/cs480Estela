#include "physics.h"

PhysicsWorld::PhysicsWorld(){
	//set up the broadphase
	broadphase = new btDbvtBroadphase();
	//set up collision configuration
	collision_config = new btDefaultCollisionConfiguration();
	//set up dispatcher
	dispatcher = new btCollisionDispatcher(collision_config);
	//set up solver
	solver = new btSequentialImpulseConstraintSolver;
	//set up the world
	dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);
	//set gravity (0,-10,0) for default
	dynamics_world->setGravity(btVector3(0,-10,0));
}

PhysicsWorld::~PhysicsWorld(){
	delete dynamics_world;
	delete broadphase;
	delete collision_config;
	delete dispatcher;
	delete solver;
}

PhysicsWorld::PhysicsWorld(btVector3 gravity){
	//set up the broadphase
	broadphase = new btDbvtBroadphase();
	//set up collision configuration
	collision_config = new btDefaultCollisionConfiguration();
	//set up dispatcher
	dispatcher = new btCollisionDispatcher(collision_config);
	//set up solver
	solver = new btSequentialImpulseConstraintSolver;
	//set up the world
	dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);
	//set the gravity
	dynamics_world->setGravity(gravity);
}

void PhysicsWorld::AddBody(btRigidBody* bd){
	dynamics_world->addRigidBody(bd,0b01111111,0b11111111);
}

void PhysicsWorld::StepSimulation(unsigned int dt){
	dynamics_world->stepSimulation(dt/1000.0f,10);
}

void PhysicsWorld::SetGravity(btVector3 g){
	dynamics_world->setGravity(g);
}

btVector3 PhysicsWorld::GetGravity(){
	return dynamics_world->getGravity();
}
