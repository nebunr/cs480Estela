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
  dynamics_world->setGravity(btVector3(0,-9.81,0));

  //Create the objects
  m_ball =  new Object(cfg.ball_config, dynamics_world);
  m_cube = new Object(cfg.cube_config, dynamics_world);
  m_cylinder = new Object(cfg.cylinder_config, dynamics_world);
  m_table = new Object(cfg.table_config, dynamics_world);

  //set up the objects vector
  objects.clear();
  objects.push_back(m_ball);
  objects.push_back(m_cube);
  objects.push_back(m_cylinder);
  objects.push_back(m_table);

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }
  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER,cfg.vshader_filename))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER,cfg.fshader_filename))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }
  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }
  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt,char input,glm::vec2 mouseLocation)
{
  //set the timestep
  //update the ball with user input
  m_cube->ProcessInput(input);
  //update all the objects
  for(unsigned int i = 0; i < objects.size(); i++){
    objects[i]->Update(dt, dynamics_world);
  }
}

void Graphics::Render()
{
  //clear the screen and sets the black background
  glClearColor(0.0, 0.0, 1.0, 1.0); //Default: (0.0, 0.0, 0.2, 1.0)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the objects
  for(unsigned int i = 0; i < objects.size(); i++)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
    objects[i]->Render();
  }

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
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

