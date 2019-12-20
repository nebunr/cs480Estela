#include "object.h"

Object::Object() {}

Object::~Object(){
	delete model;
	delete texture;
	delete shape;
	delete body;
	//delete triangle_mesh;
	delete motion_state;
}

Object::Object(char* object_config_filename){
	//start with model matrix as idenity matrix
	model_matrix = glm::mat4(1.0f);
	//set up the object config
	LoadObjectConfig(object_config_filename);
	//Load the model
	model = new Model(config.model_filename);
	//Load the texture
	texture = new Texture(config.texture_filename);
	//Translate model by initial level1Pos from config
	model_matrix *= glm::translate(glm::mat4(1.0f),config.level1Pos);
	//set the model scale
	model_matrix *= glm::scale(glm::mat4(1.0f),glm::vec3(config.scale));
	//Load the rigid body
	LoadBody(config.shape);
}

void Object::ProcessInput(char input) {}

void Object::Update(unsigned int dt){
	btTransform transform ;
	body->getMotionState()->getWorldTransform(transform);

	btScalar m[16];
	transform.getOpenGLMatrix(m);

	model_matrix = glm::make_mat4(m);
	model_matrix *= glm::scale(glm::vec3(config.scale));
	location = glm::vec3(m[12],m[13],m[14]);
}

void Object::Render(){
	texture->Render();
	model->Render();
}

glm::mat4 Object::GetModelMatrix(){
	return model_matrix;
}

glm::vec3 Object::GetLocation(){
	return location;
}

void Object::LoadObjectConfig(char* object_config_filename){
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError file_loaded = doc.LoadFile(object_config_filename);
	if(file_loaded != tinyxml2::XML_SUCCESS){
		std::string error;
		std::string filename(object_config_filename);
		error = "XML Error: " + std::to_string(file_loaded) + " in file: "+ filename + "\n";
		throw std::logic_error(error);
	}
	tinyxml2::XMLElement* object = doc.FirstChildElement("object");
	tinyxml2::XMLElement* element = NULL;

	//set the model
	if((element = object->FirstChildElement("model"))){
		config.model_filename = new char[256];
		strcpy(config.model_filename, element->GetText());
	}
	//set the texture
	if((element = object->FirstChildElement("texture"))){
		config.texture_filename = new char[256];
		strcpy(config.texture_filename, element->GetText());
	}
	//set the scale
	if((element = object->FirstChildElement("scale"))){
		config.scale = element->FloatText();
	}
	//set the mass
	if((element = object->FirstChildElement("mass"))){
		config.mass = element->FloatText();
	}
	//set the shininess
	if((element = object->FirstChildElement("shininess"))){
		float m = element->FloatText();
		config.shininess = m;
	}
	//set the friction
	if((element = object->FirstChildElement("friction"))){
		config.friction = element->FloatText();
	}
	//set the restitution
	if((element = object->FirstChildElement("restitution"))){
		config.restitution = element->FloatText();
	}
	//set the is_kinematic flag
	if((element = object->FirstChildElement("kinematic"))){
		config.is_kinematic = element->BoolText();
	}
	//set the shape
	if((element = object->FirstChildElement("shape"))){
		config.shape = new char[100];
		strcpy(config.shape,element->GetText());
	}

	//set the initial position
	float x,y,z;
	x = y = z = 0;
	if((element = object->FirstChildElement("level1-pos"))){
		tinyxml2::XMLElement* position = NULL;
		//set the x,y,z
		if((position = element->FirstChildElement("x"))){
			x = position->FloatText();
		}
		if((position = element->FirstChildElement("y"))){
			y = position->FloatText();
		}
		if((position = element->FirstChildElement("z"))){
			z = position->FloatText();
		}
	}
	config.level1Pos = (glm::vec3(x,y,z));	

	x = y = z = 0;
	if((element = object->FirstChildElement("level2-pos"))){
		tinyxml2::XMLElement* position = NULL;
		//set the x,y,z
		if((position = element->FirstChildElement("x"))){
			x = position->FloatText();
		}
		if((position = element->FirstChildElement("y"))){
			y = position->FloatText();
		}
		if((position = element->FirstChildElement("z"))){
			z = position->FloatText();
		}
	}
	config.level2Pos = (glm::vec3(x,y,z));	

	x = y = z = 0;
	if((element = object->FirstChildElement("level3-pos"))){
		tinyxml2::XMLElement* position = NULL;
		//set the x,y,z
		if((position = element->FirstChildElement("x"))){
			x = position->FloatText();
		}
		if((position = element->FirstChildElement("y"))){
			y = position->FloatText();
		}
		if((position = element->FirstChildElement("z"))){
			z = position->FloatText();
		}
	}
	config.level3Pos = (glm::vec3(x,y,z));

}

void Object::LoadBody(char* shape_str){
	//load the shape
	LoadShape(shape_str);
	//setup initial stransform
	btTransform start_transform;
	start_transform.setIdentity();
	//set mass and inertia
	btScalar mass = btScalar(config.mass);
	bool is_dynamic = (mass != 0.0f);
	btVector3 local_inertia(0,0,0);
	if(is_dynamic){
		shape->calculateLocalInertia(mass,local_inertia);
	}
	//set transform origin to object's initial position
	start_transform.setOrigin(btVector3(config.level1Pos.x,config.level1Pos.y,config.level1Pos.z));
	//set up the rigid body
	motion_state = new btDefaultMotionState(start_transform);
	btRigidBody::btRigidBodyConstructionInfo rigid_body_information(mass,motion_state,shape,local_inertia);
	//set friction and restitution
	rigid_body_information.m_restitution = config.restitution;
	rigid_body_information.m_friction = config.friction;
	body = new btRigidBody(rigid_body_information);
	//dynamic object
	if(is_dynamic){
		body->setActivationState(DISABLE_DEACTIVATION);
	}
	//kinematic object
	else if(config.is_kinematic){
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}
	//static object
	else{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}
	//set friction
	body->setFriction(btScalar(config.friction));
}

void Object::LoadShape(char* shape_str){
	if((strcmp(shape_str,"mesh"))==0){
		if(config.mass == 0){
			btBvhTriangleMeshShape* temp_shape = new btBvhTriangleMeshShape(model->GetTriangleMesh(),true);
			shape = new btScaledBvhTriangleMeshShape(temp_shape,btVector3(config.scale,config.scale,config.scale));
		}
		else{
			shape = new btConvexTriangleMeshShape(model->GetTriangleMesh(), true);
			shape->setLocalScaling(btVector3(config.scale,config.scale,config.scale));
		}
	}
	else if((strcmp(shape_str,"plane"))==0){}
	else if((strcmp(shape_str,"box"))==0){}
	else if((strcmp(shape_str,"cylinder"))==0){}
	else if((strcmp(shape_str,"sphere"))==0){
		btScalar radius = config.scale;
		shape = new btSphereShape(radius);
	}
	else if((strcmp(shape_str,"skybox"))==0){
		btVector3 vec = {0, 0, 0};
		shape = new btBoxShape(vec);
	}
}

btRigidBody* Object::GetRigidBody(){
	return body;
}
float Object::GetShininess(){
 	return config.shininess;
}

void Object::SetLocationLevel(int level)
{
  
  btTransform newTransform;

  if(level == 0)
  {
 	 newTransform.setOrigin(btVector3(config.level1Pos.x, config.level1Pos.y, config.level1Pos.z));
 	 newTransform.setRotation(btQuaternion(0,1,0,1));
  }
  if(level == 1)
  {
 	 newTransform.setOrigin(btVector3(config.level2Pos.x, config.level2Pos.y, config.level2Pos.z));
 	 newTransform.setRotation(btQuaternion(0,1,0,1));
  }
  if(level == 2)
  {
 	 newTransform.setOrigin(btVector3(config.level3Pos.x, config.level3Pos.y, config.level3Pos.z));
 	 newTransform.setRotation(btQuaternion(0,1,0,1));  
  }

  body->setWorldTransform(newTransform);
  body->setCenterOfMassTransform(newTransform);
  motion_state->setWorldTransform(newTransform);
  
}

//last minute way to check if the ball has fallen into the hole
bool Object::CheckLevelWin(int level)
{
	if(location.y < -2)
	{
		if(level == 0)
		{
			if(location.x >= -46 && location.x <= -39)
			{
				if(location.z >= -4 && location.z <= 6)
				{
					return true;
				}
			}
		}
	}
	if(location.y < 20 && location.y > 16)
	{
		if(level == 1)
		{
			//test this more
			if(location.x >= -173 && location.x <= -168)
			{
				if(location.z >= -6 && location.z <= -1)
				{
					return true;
				}
			}
		}
	}
	//ben: make level == 2
	if(location.y < 23)
	{
		if(level == 2)
		{
			//std::cout << "x: " << location.x << " y: " << location.y << " z: " << location.z << std::endl;
			//test this more
			if(location.x >= -257 && location.x <= -249)
			{
				if(location.z >= -2 && location.z <= 1)
				{
					//std::cout << "AAAAAAAAAAAAAAAAAAAAA" << std::endl;
					return true;
				}
			}
		}
	}
	return false;
}