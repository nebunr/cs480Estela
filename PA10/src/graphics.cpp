#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
  delete dynamics_world;
  delete broadphase;
  delete collision_config;
  delete dispatcher;
  delete solver;  
}

bool Graphics::Initialize(int width, int height, Config cfg)
{
  //set up ambient lighting
  ambient_light_color.r = cfg.ar;
  ambient_light_color.g = cfg.ag;
  ambient_light_color.b = cfg.ab;
  ambient_light_intensity = cfg.ambient_intensity;
    
  specular_intensity = cfg.specular_intensity;
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();


    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif
  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  //Create the physics world
  //set up broadphase
  broadphase = new btDbvtBroadphase();

  //set up collision configuration
  collision_config = new btDefaultCollisionConfiguration();

  //set up dispatcher
  dispatcher = new btCollisionDispatcher(collision_config);

  //set up solver
  solver = new btSequentialImpulseConstraintSolver;

  //set up the world
  dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config );

  //set gravity
  dynamics_world->setGravity(btVector3(0,-10,10));

  //Create the objects
  m_table = new Table(cfg.table_config);
  m_ball = m_table->GetBall();

  //set up the objects vector
  objects.clear();
  objects = m_table->GetObjects();

  //add all rigit bodies to physics world
  for(unsigned int i = 0; i < objects.size(); i++){
    dynamics_world->addRigidBody(objects[i]->GetRigidBody(),0b01111111,0b11111111);
    objects[i]->GetRigidBody()->setUserPointer(objects[i]);
  }
  
  //set up plane
  btTransform planeTransform;
  planeTransform.setIdentity();
  planeTransform.setOrigin(btVector3(0,6,4));
  btScalar mass = 0;
  btVector3 inertia(0,0,0);
  btStaticPlaneShape* shape = new btStaticPlaneShape(btVector3(0,1,0),100);
  btDefaultMotionState* shape_state = new btDefaultMotionState(planeTransform);
  btRigidBody::btRigidBodyConstructionInfo rigid_body_information(mass,shape_state,shape,inertia);
  

  //set up the spotlight;
  spotlight.intensity = cfg.spotlightIntensity;
  cutOffDivider = cfg.spotlightCutoff;
  spotlight.cutoff = M_PI / cutOffDivider;
  spotlight.position = glm::vec3(cfg.sx, cfg.sy, cfg.sz);
  spotlight.color = glm::vec3(cfg.sr, cfg.sg, cfg.sb);

  plungerIntensity = -40000;

  // Set up game logic
  MAX_SCORE = cfg.extraGameScore;
  score = cfg.startScore;
  lives = cfg.startLives;
  gameOver = false;
  gameOverPrinted = false;

  // Set up the shaders'
  Shader* s1 = new Shader();
  Shader* s2 = new Shader();

  if(!s1->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }
  // Add the vertex shader
  if(!s1->AddShader(GL_VERTEX_SHADER,cfg.vshader1_filename))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!s1->AddShader(GL_FRAGMENT_SHADER,cfg.fshader1_filename))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }
  // Connect the program
  if(!s1->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  //push the program to the shaders vertex
  m_shaders.push_back(s1);
  
  if(!s2->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }
  // Add the vertex shader
  if(!s2->AddShader(GL_VERTEX_SHADER,cfg.vshader2_filename))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!s2->AddShader(GL_FRAGMENT_SHADER,cfg.fshader2_filename))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }
  // Connect the program
  if(!s2->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  m_shaders.push_back(s2);

  shader_index = 0;

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shaders[shader_index]->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shaders[shader_index]->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  m_modelMatrix = m_shaders[shader_index]->GetUniformLocation("modelMatrix");
  // Locate the model matrix in the shader
  m_modelViewMatrix = m_shaders[shader_index]->GetUniformLocation("modelViewMatrix");
  m_lightPosition = m_shaders[shader_index]->GetUniformLocation("AmbientLightPosition");
  m_shininess = m_shaders[shader_index]->GetUniformLocation("shininess");
  m_ambient = m_shaders[shader_index]->GetUniformLocation("AmbientProduct");
  m_diffuse = m_shaders[shader_index]->GetUniformLocation("DiffuseProduct");
  m_specular = m_shaders[shader_index]->GetUniformLocation("SpecularProduct");
  m_specular_intensity = m_shaders[shader_index]->GetUniformLocation("SpecularIntensity");
  m_viewPos = m_shaders[shader_index]->GetUniformLocation("viewPos");
  m_ambient_color = m_shaders[shader_index]->GetUniformLocation("AmbientLightColor");
  m_ambient_intensity = m_shaders[shader_index]->GetUniformLocation("AmbientLightIntensity");
  m_spotlight_position = m_shaders[shader_index]->GetUniformLocation("spotlightPosition");
  m_spotlight_direction = m_shaders[shader_index]->GetUniformLocation("spotlightDirection");
  m_spotlight_cutoff = m_shaders[shader_index]->GetUniformLocation("spotlightCutoff");
  m_spotlight_color = m_shaders[shader_index]->GetUniformLocation("spotlightColor");
  m_spotlight_intensity = m_shaders[shader_index]->GetUniformLocation("spotlightIntensity");
  shader_index = 0;
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //Locate lighting uniforms in the shader
  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //flip all the flippers to make sure none of them are bugged out
  m_table->FlipLeftFlippers();
  m_table->FlipRightFlippers();

  //Setup file io for pinball scoreboard
  ifstream scoreboard ("../assets/scoreboard.txt");
  if(scoreboard.is_open())
  {
    std::cout << std::endl << scoreboard.rdbuf() << std::endl;
    scoreboard.close();
  }
  else
  {
    std::cout << "Could not open ../assets/scoreboard.txt" << std::endl;
  }
  
  printf("Welcome to pinball!\n");
  printf("Press c to shoot!\n");
  printf("Press z and x for the flippers!\n\n");
  printf("Balls left: %d\n", lives);
  printf("Score: %d\n\n", score);

  return true;
}

void Graphics::Update(unsigned int dt,char input,glm::vec2 mouseLocation)
{
  //handles plunger, flippers, and shader changes 

  dynamics_world->stepSimulation(dt/1000.0f,10);

  Input(input);

  m_table->Update(dt);
  
  if( m_ball->GetLocation().x >= -16 && m_ball->GetLocation().x <= 16 &&
      m_ball->GetLocation().y >= 3 && m_ball->GetLocation().y <= 15 &&
      m_ball->GetLocation().z >= -20 && m_ball->GetLocation().z <= 34 )
  {
    lastLocation = m_ball->GetLocation();
  }
  else
  {
    //printf("Last Seen: %f, %f, %f\n", lastLocation.x, lastLocation.y, lastLocation.z);
    //m_ball->SetLocation(lastLocation);
    //printf("Where am I?: %f, %f, %f\n", m_ball->GetLocation().x, m_ball->GetLocation().y, m_ball->GetLocation().z);
  }

  if( (m_ball->GetLocation().x >= 13.5) && m_ball->GetLocation().x <= 15.9 && m_ball->GetLocation().z >= 20 )
  {
    plungerInZone = true;
  }
  else
  {
    plungerInZone = false;
  }
  

  // lose life / game over
  livesChanged = false;
  scoreChanged = false;
  if( m_ball->GetLocation().z >= 34 && m_ball->GetLocation().x <= 13.8 )
  {
    lives--;
    m_ball->SetLocation(glm::vec3(14.6, 3.5, 22.8));
    livesChanged = true;
  }
  if (m_ball->GetLocation().y >= 7)
  {
    m_ball->SetLocation(glm::vec3(m_ball->GetLocation().x, 4, m_ball->GetLocation().z));
  }
  if(lives <= 0)
  { 
    if(!gameOverPrinted)
    {
      gameOver = true;
      printf("Game over!\n");
      printf("Final score: %d\n", score);
      printf("Press p to replay!\n\n");
      gameOverPrinted = true;
    }
  }



  // Check collisions
  int numManifolds = dynamics_world->getDispatcher()->getNumManifolds();
  for( int i = 0; i < numManifolds; i++ )
  {
    btPersistentManifold* contactManifold = dynamics_world->getDispatcher()->getManifoldByIndexInternal(i);
    const btCollisionObject* obA = contactManifold->getBody0();
    const btCollisionObject* obB = contactManifold->getBody1();
    std::vector<Object*> bumpers = m_table->GetBumpers();
    
    if((obA->getUserPointer()) == m_ball)
    {
      for(int i = 0; i < bumpers.size(); i++){
        if(obB->getUserPointer() == bumpers[i]){
          score++;
          scoreChanged = true;
	}
      }
    }
    if((obB->getUserPointer()) == m_ball)
    {
      for(int i = 0; i < bumpers.size(); i++){
        if(obA->getUserPointer() == bumpers[i]){
          score++;
          scoreChanged = true;
	}
      }
    }

    if(!gameOver)
    {
      // check for extra ball
      if( score % MAX_SCORE == 0 && score != 0)
      {
        lives++;
        livesChanged = true;
      }

      // Print your states
      if(livesChanged || scoreChanged)
      {
        printf("Balls left: %d\n", lives);
        printf("Score: %d\n\n", score);
      }
    }    

    if(plungerChanged)
    {
      printf("Plunger intensity: %f\n", plungerIntensity / -10000 - 4);   
      plungerChanged = false;   
    }
  }

  //set the timestep
  //update the ball with user input
  //m_cube->ProcessInput(input);
  //update all the objects
  for(unsigned int i = 0; i < objects.size(); i++){
    objects[i]->Update(dt, dynamics_world);
  }
  //update spotlight direction and position
  spotlight.direction = glm::vec3(0, -1, 0);
  spotlight.position = m_ball->GetLocation();
  spotlight.position.y = 10;

}

void Graphics::Render()
{
  
  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shaders[shader_index]->GetUniformLocation("projectionMatrix");
  m_viewMatrix = m_shaders[shader_index]->GetUniformLocation("viewMatrix");
  m_modelMatrix = m_shaders[shader_index]->GetUniformLocation("modelMatrix");
  // Locate the model matrix in the shader
  m_modelViewMatrix = m_shaders[shader_index]->GetUniformLocation("modelViewMatrix");
  m_lightPosition = m_shaders[shader_index]->GetUniformLocation("AmbientLightPosition");
  m_shininess = m_shaders[shader_index]->GetUniformLocation("shininess");
  m_ambient = m_shaders[shader_index]->GetUniformLocation("AmbientProduct");
  m_diffuse = m_shaders[shader_index]->GetUniformLocation("DiffuseProduct");
  m_specular = m_shaders[shader_index]->GetUniformLocation("SpecularProduct");
  m_specular_intensity = m_shaders[shader_index]->GetUniformLocation("SpecularIntensity");
  m_viewPos = m_shaders[shader_index]->GetUniformLocation("viewPos");
  m_ambient_color = m_shaders[shader_index]->GetUniformLocation("AmbientLightColor");
  m_ambient_intensity = m_shaders[shader_index]->GetUniformLocation("AmbientLightIntensity");
  m_spotlight_position = m_shaders[shader_index]->GetUniformLocation("spotlightPosition");
  m_spotlight_direction = m_shaders[shader_index]->GetUniformLocation("spotlightDirection");
  m_spotlight_cutoff = m_shaders[shader_index]->GetUniformLocation("spotlightCutoff");
  m_spotlight_color = m_shaders[shader_index]->GetUniformLocation("spotlightColor");
  m_spotlight_intensity = m_shaders[shader_index]->GetUniformLocation("spotlightIntensity");
  //clear the screen and sets the black background
  glClearColor(0.0, 0.0, 0.0, 1.0); //Default: (0.0, 0.0, 0.2, 1.0)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Start the correct program
  m_shaders[shader_index]->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the objects
  for(unsigned int i = 0; i < objects.size(); i++)
  {
    glm::mat4 modelView = m_camera->GetView() * objects[i]->GetModel();
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
    glUniformMatrix4fv(m_modelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelView));
    glUniform3fv(m_lightPosition, 1, glm::value_ptr(glm::vec3(0.0,5.0,0.0)));
    glUniform1f(m_shininess, objects[i]->GetShininess());
    glUniform4fv(m_ambient, 1, glm::value_ptr(objects[i]->GetAmbient()));
    glUniform4fv(m_diffuse, 1, glm::value_ptr(objects[i]->GetDiffuse()));
    glUniform4fv(m_specular, 1, glm::value_ptr(objects[i]->GetSpecular()));
    glUniform3fv(m_viewPos, 1, glm::value_ptr(m_camera->GetPos()));
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

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
  }
}

void Graphics::Input(char input)
{
  if(input == '\t'){
    shader_index++;
    shader_index %= m_shaders.size();
  }
  if(input == 'z'){
    if(!gameOver)
      m_table->FlipLeftFlippers();
  }
  if(input == 'x'){
    if(!gameOver)
      m_table->FlipRightFlippers();
  }
  if(input == 'c'){
    if( plungerInZone ){ //check if ball is in plunger area  
      // -40000 = minimum
      // -50000 = maximum
      if(!gameOver)
      {
        m_ball->applyForce(btVector3(0, 0, plungerIntensity));
      }
    }
  }

  if(input == 'n')
  {
    if( plungerInZone )
    {
      plungerIntensity += 1000;
      if (plungerIntensity >= -40000)
      {
        plungerIntensity = -40000;
      }
      plungerChanged = true;
    }
  }
  if(input == 'm')
  {
    if(plungerInZone)
    {
      plungerIntensity -= 1000;
      if(plungerIntensity <= -60000)
      {
        plungerIntensity = -60000;
      }
    }
    plungerChanged = true;
  }

  if(input == 'b'){
    printf("Resetting Ball\n");
    m_ball->SetLocation(glm::vec3(14.6,3.5,22.8));
    m_ball->GetRigidBody()->clearForces();
    m_ball->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
    m_ball->GetRigidBody()->setAngularVelocity(btVector3(0,0,0));
  }
  if(input == 'p'){
    if(gameOver)
    {
      for(int i = 0; i < 60; i++)
      {
        printf("\n");
      }
      lives = 3;
      score = 0;
      gameOver = !gameOver;
      printf("Welcome to pinball!\n");
      printf("Press c to shoot!\n");
      printf("Press z and x for the flippers!\n\n");
      printf("Balls left: %d\n", lives);
      printf("Score: %d\n\n", score);
      gameOverPrinted = false;
    }
  }

  // Specular change
  if(input == 'w')
  {
    specular_intensity += 0.1;
  }
  if(input == 's')
  {
    specular_intensity -= 0.1;
    if(specular_intensity <= 0)
      specular_intensity = 0;
  } 
  if(input == '^')
  {
    ambient_light_intensity += 0.1;
  }
  if(input == 'V')
  {
    ambient_light_intensity -= 0.1;
  }

  if(input == 'i')
  {
    spotlight.intensity += 10;
  }

  if(input == 'k')
  {
    spotlight.intensity -= 10;
    if(spotlight.intensity <= 0)
      spotlight.intensity = 0;
  }

  if(input == 'l')
  {
    cutOffDivider -= 1;
    if(cutOffDivider <= 1)
    {
      cutOffDivider = 1;
    }
    
    spotlight.cutoff = M_PI / cutOffDivider;
  }

  if(input == 'j')
  {
    cutOffDivider += 1;
    spotlight.cutoff = M_PI / cutOffDivider;
  }

  if(input == 'r')
  {
    printf("Ball location: %f, %f, %f\n", m_ball->GetLocation().x, m_ball->GetLocation().y, m_ball->GetLocation().z);
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

