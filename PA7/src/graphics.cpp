#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

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

  //Create the object
  m_cube =  new Object(cfg.root_planet_filename);

  //set up the objects and followable_objects vectors
  objects.clear();
  objects.push_back(m_cube);

  followable_objects.clear();
  followable_objects.push_back(m_cube);

  std::vector<Object*> sat = m_cube->getSatelites();

  for(unsigned int i = 0; i < sat.size(); i++){
    std::vector<Object*> sats = sat[i]->getSatelites();
    objects.push_back(sat[i]);
    followable_objects.push_back(sat[i]);
    for(unsigned int j = 0; j < sats.size(); j++){
      objects.push_back(sats[j]);
    }
  }
  followable_objects.push_back(NULL);

  sat.clear();
  selected_index = 0;
  following_index = followable_objects.size() - 1;
  
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

  flat_earth = false; // Yes, the earth is round.

  return true;
}

void Graphics::Update(unsigned int dt,char input,glm::vec2 mouseLocation)
{
  // To scroll through planets
  if(input == '\t'){
    selected_index++;
    selected_index = selected_index % objects.size();
    std::cout << "Selected Object: " << objects[selected_index]->getName() << std::endl;
  }
  if(std::isdigit(input)){
    following_index = input - '0';
  }
  // Update objects
  objects[selected_index]->processInput(input);
  // Enables flatness for just Earth
  if(input == 'f' && selected_index == 3)
  {
    HandleFlatEarth();
  }
  // Update selected object
  for(unsigned int i = 0; i < objects.size(); i++)
  {
    if(i == 3)
    {
      objects[i]->Update(dt, flat_earth);
    }
    else
    {
      objects[i]->Update(dt, false);
    }
    
  }
  
  // Update m_camera
  m_camera->Input(input, dt);
  m_camera->Update(dt, mouseLocation);
}

void Graphics::Render()
{
  //clear the screen and sets the black background
  glClearColor(0.0, 0.0, 0.0, 1.0); //Default: (0.0, 0.0, 0.2, 1.0)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the objects
  for(unsigned int i = 0; i < objects.size(); i++){
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

void Graphics::HandleFlatEarth()
{
  if(flat_earth == false)
  {
    flat_earth = true;
    std::cout << 
    std::endl <<
    "What is the Flat Earth Society?" << 
    std::endl <<
    "The Flat Earth Society is a group actively promoting the Flat Earth Movement worldwide. " <<
    "Descending from Samuel Shenton's International Flat Earth Research Society, and the " << 
    "Universal Zetetic Society before it, we continue the age-old tradition of questioning " << 
    "the Round Earth doctrine and challenging authorities. Acknowledging the link between " <<
    "various unconventional beliefs, the Society also occasionally engages in other " <<
    "controversial debates, striving to provide a voice for all free thinkers and Zeteticists." << 
    std::endl <<
    "(Source: wiki.tfes.org)" <<
    std::endl <<
    "What evidence do you have?" << 
    std::endl <<
    "Just look at that solar system..." << 
    std::endl;
  }
  else
  {
    flat_earth = false;
    for(int i = 0; i < 60; i++)
    {
      std::cout << std::endl;
    }
    std::cout << "The Earth is in fact, round. Uh, clear that terminal of falseness." 
              << std::endl;
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

