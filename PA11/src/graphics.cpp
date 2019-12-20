#include "graphics.h"

Graphics::Graphics(){}
Graphics::~Graphics()
{
	/*
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	*/
}

bool Graphics::Initialize(int w, int h, Config cfg){
	//set up ambient lighting
	ambient_light_color.r = cfg.ar;
	ambient_light_color.g = cfg.ag;
	ambient_light_color.b = cfg.ab;
	ambient_light_intensity = cfg.ambient_intensity;
		
	specular_intensity = cfg.specular_intensity;
	
	//used for linux OS
	#if !defined(__APPLE__) && !defined(MACOSX)
		glewExperimental = GL_TRUE;
		auto status = glewInit();
		glGetError();
		if(status != GLEW_OK){
			std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
			return false;
		}
	#endif
	//For OpenGL 3
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Init Camera
	camera = new Camera();
	if(!camera->Initialize(w,h)){
		printf("Camera Failed to Initialize\n");
		return false;
	}

	//set up the spotlight;
	spotlight.intensity = cfg.spotlightIntensity;
	cutOffDivider = cfg.spotlightCutoff;
	spotlight.cutoff = M_PI / cutOffDivider;
	spotlight.position = glm::vec3(cfg.sx, cfg.sy, cfg.sz);
	spotlight.color = glm::vec3(cfg.sr, cfg.sg, cfg.sb);

	//Set up the shaders
	Shader* s1 = new Shader();
	if(!s1->Initialize()){
		printf("Shader Failed to Initialize\n");
		return false;
	}
	//Add the vertex shader
	if(!s1->AddShader(GL_VERTEX_SHADER,cfg.vshader_filename)){
		printf("Vertex Shader Failed to Initialize\n");
	}
	//Add the fragment shader
	if(!s1->AddShader(GL_FRAGMENT_SHADER,cfg.fshader_filename)){
		printf("Fragment Shader failed to Initialize\n");
	}
	//Connect the program
	if(!s1->Finalize()){
		printf("Program failed to Finalize\n");
		return false;
	}
	shaders.push_back(s1);

	shader_index = 0;

	//Locate the MVP matricies in the shader
	projection_matrix = shaders[0]->GetUniformLocation("projectionMatrix");
	if(projection_matrix == INVALID_UNIFORM_LOCATION){
		printf("projectionMatrix not found\n");
	}
	view_matrix = shaders[0]->GetUniformLocation("viewMatrix");
	if(view_matrix == INVALID_UNIFORM_LOCATION){
		printf("viewMatrix not found\n");
	}
	model_matrix = shaders[0]->GetUniformLocation("modelMatrix");
	if(model_matrix == INVALID_UNIFORM_LOCATION){
		printf("modelMatrix not found\n");
	}

	m_lightPosition = shaders[shader_index]->GetUniformLocation("AmbientLightPosition");
	m_shininess = shaders[shader_index]->GetUniformLocation("shininess");
	m_ambient = shaders[shader_index]->GetUniformLocation("AmbientProduct");
	m_diffuse = shaders[shader_index]->GetUniformLocation("DiffuseProduct");
	m_specular = shaders[shader_index]->GetUniformLocation("SpecularProduct");
	m_specular_intensity = shaders[shader_index]->GetUniformLocation("SpecularIntensity");
	m_viewPos = shaders[shader_index]->GetUniformLocation("viewPos");
	m_ambient_color = shaders[shader_index]->GetUniformLocation("AmbientLightColor");
	m_ambient_intensity = shaders[shader_index]->GetUniformLocation("AmbientLightIntensity");
	m_spotlight_position = shaders[shader_index]->GetUniformLocation("spotlightPosition");
	m_spotlight_direction = shaders[shader_index]->GetUniformLocation("spotlightDirection");
	m_spotlight_cutoff = shaders[shader_index]->GetUniformLocation("spotlightCutoff");
	m_spotlight_color = shaders[shader_index]->GetUniformLocation("spotlightColor");
	m_spotlight_intensity = shaders[shader_index]->GetUniformLocation("spotlightIntensity");

	//set up the physics world
	physics_world = new PhysicsWorld(btVector3(10,-10,0));

	//set up the objects
	//ball must be objects[1]
	Object* level1 = new Object("../assets/configs/apesphere/level1.xml");
	Object* ball = new Object("../assets/configs/apesphere/ball.xml");
	Object* level2 = new Object("../assets/configs/apesphere/level2.xml");
	Object* level3 = new Object("../assets/configs/apesphere/level3.xml");
	Object* harris = new Object("../assets/configs/apesphere/harris.xml");
	Object* skyBox = new Object("../assets/configs/apesphere/skybox.xml");
	
	objects.push_back(level1);
	objects.push_back(ball);
	objects.push_back(level2);
	objects.push_back(level3);
	objects.push_back(harris);
	objects.push_back(skyBox);
	

	//add the objects to the physics world
	for(unsigned int i = 0; i < objects.size(); i++){
		physics_world->AddBody(objects[i]->GetRigidBody());
	}

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	objects[1]->SetLocationLevel(0);

	return true;
}

void Graphics::Update(unsigned int dt, KeyHandler* key_handler, bool& died, bool& win, int level){
	camera->Update(dt,key_handler,objects[1]);
	glm::vec3 g = camera->GetGravity();
	g *= 5;
	physics_world->SetGravity(btVector3(g.x,g.y,g.z));

	physics_world->StepSimulation(dt);
	for(unsigned int i = 0; i < objects.size(); i++){
		objects[i]->Update(dt);
	}

	// friction simulation
	btScalar linearVelocity_x = objects[1]->GetRigidBody()->getLinearVelocity().getX();
	btScalar linearVelocity_y = objects[1]->GetRigidBody()->getLinearVelocity().getY();
	btScalar linearVelocity_z = objects[1]->GetRigidBody()->getLinearVelocity().getZ();
	if( linearVelocity_x > 0 )
	{
		linearVelocity_x -= float(dt)/5000.f;
		if( linearVelocity_x <= 0 )
			linearVelocity_x = 0;
	}
	if( linearVelocity_x < 0 )
	{
		linearVelocity_x += float(dt)/5000.f;
		if( linearVelocity_x >= 0 )
			linearVelocity_x = 0;
	}
		if( linearVelocity_z > 0 )
	{
		linearVelocity_z -= float(dt)/5000.f;
		if( linearVelocity_z <= 0 )
			linearVelocity_z = 0;
	}
	if( linearVelocity_z < 0 )
	{
		linearVelocity_z += float(dt)/5000.f;
		if( linearVelocity_z >= 0 )
			linearVelocity_z = 0;
	}

	objects[1]->GetRigidBody()->setLinearVelocity(btVector3(linearVelocity_x, linearVelocity_y, linearVelocity_z));

	//basic game logic if ball falls off, the ball is reset
	if(objects[1]->GetLocation().y < -10)
	{
		Reset(level);
		died = true;			
	}

	if(objects[1]->CheckLevelWin(level))
	{
		win = true;
		Reset(level);
	}
	//printf("%f, %f, %f\n", objects[1]->GetLocation().x, objects[1]->GetLocation().y, objects[1]->GetLocation().z);
}

void Graphics::Render(){
	m_lightPosition = shaders[shader_index]->GetUniformLocation("AmbientLightPosition");
	m_shininess = shaders[shader_index]->GetUniformLocation("shininess");
	m_ambient = shaders[shader_index]->GetUniformLocation("AmbientProduct");
	m_diffuse = shaders[shader_index]->GetUniformLocation("DiffuseProduct");
	m_specular = shaders[shader_index]->GetUniformLocation("SpecularProduct");
	m_specular_intensity = shaders[shader_index]->GetUniformLocation("SpecularIntensity");
	m_viewPos = shaders[shader_index]->GetUniformLocation("viewPos");
	m_ambient_color = shaders[shader_index]->GetUniformLocation("AmbientLightColor");
	m_ambient_intensity = shaders[shader_index]->GetUniformLocation("AmbientLightIntensity");
	m_spotlight_position = shaders[shader_index]->GetUniformLocation("spotlightPosition");
	m_spotlight_direction = shaders[shader_index]->GetUniformLocation("spotlightDirection");
	m_spotlight_cutoff = shaders[shader_index]->GetUniformLocation("spotlightCutoff");
	m_spotlight_color = shaders[shader_index]->GetUniformLocation("spotlightColor");
	m_spotlight_intensity = shaders[shader_index]->GetUniformLocation("spotlightIntensity");
	
	//clear the screen and set the black background
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//start the shader
	shaders[0]->Enable();

	//send in the view and projection matrices to the shader
	glUniformMatrix4fv(projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
	glUniformMatrix4fv(view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	//Render the objects
	for(unsigned int i = 0; i < objects.size(); i++){
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModelMatrix()));
		glUniform3fv(m_lightPosition, 1, glm::value_ptr(glm::vec3(0.0,10.0,0.0)));
		glUniform1f(m_shininess, objects[i]->GetShininess());
		glUniform3fv(m_viewPos, 1, glm::value_ptr(camera->GetPos()));
		glUniform1f(m_specular_intensity, specular_intensity);
		glUniform3fv(m_ambient_color, 1, glm::value_ptr(ambient_light_color));
		glUniform1f(m_ambient_intensity, ambient_light_intensity);
		glUniform3fv(m_spotlight_position, 1, glm::value_ptr(spotlight.position));
		glUniform3fv(m_spotlight_direction, 1, glm::value_ptr(spotlight.direction));
		glUniform3fv(m_spotlight_color, 1, glm::value_ptr(spotlight.color));
		glUniform1f(m_spotlight_cutoff, glm::cos(spotlight.cutoff));
		glUniform1f(m_spotlight_intensity, spotlight.intensity);
		
		objects[i]->Render();
	}

	//Get any errors from OpenGL
	auto error = glGetError();
	if(error != GL_NO_ERROR){
		printf("%s",ErrorString(error));
	}
}

char* Graphics::ErrorString(GLenum error){
	char ret[6];
	printf("Graphics::ErrorString function called: ");
	return strcpy(ret,"test"); //Test this!
}	

void Graphics::Reset(int level)
{
	// the ball is reset
	physics_world->SetGravity(btVector3(0,-10,0));
	objects[1]->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
	objects[1]->GetRigidBody()->setAngularVelocity(btVector3(0,0,0));
	objects[1]->SetLocationLevel(level);
	camera->Reset();
}