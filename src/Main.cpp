#include "Main.hpp"
#include "shader_util.hpp"
#include "gl_framework.hpp"
#include "camera.hpp"
#include "Input.hpp"

camera Vox_cam;
GLuint mat1,mat2, matl1, matl2, matc1, matc2;
GLuint shaderProgram, linesProgram, cursorProgram;

std::vector<glm::vec3> data, linedata, cursordata;
std::vector<unsigned short> ebo;
std::vector<unsigned short> cursor_indice;

float curr_frame_time, last_frame_time = 0.0f, delta_time;

unsigned int cube_vao, cube_vbo, cube_ebo, lines_vao, lines_vbo, cursor_vao, cursor_vbo, cursor_ebo;

void set_cube_buf()
{
    glBindVertexArray (cube_vao);
    glBindBuffer (GL_ARRAY_BUFFER, cube_vbo);
    glBufferData (GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), &(data[0]), GL_STATIC_DRAW); 
}

void set_cursor_buf()
{
    glBindVertexArray (cursor_vao);
    glBindBuffer (GL_ARRAY_BUFFER, cursor_vbo);
    glBufferData (GL_ARRAY_BUFFER, cursordata.size()*sizeof(glm::vec3), &(cursordata[0]), GL_STATIC_DRAW);
}

void initShadersGL(void)
{
  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, "simple_vs.glsl"));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, "simple_fs.glsl"));

  shaderProgram = csX75::CreateProgramGL(shaderList);

  glUseProgram(shaderProgram);
  mat1 = glGetUniformLocation(shaderProgram, "view");
  mat2 = glGetUniformLocation(shaderProgram, "proj");

  shaderList = {
    csX75::LoadShaderGL(GL_VERTEX_SHADER, "line_vs.glsl"),
    csX75::LoadShaderGL(GL_FRAGMENT_SHADER, "line_fs.glsl")
  };

  linesProgram = csX75::CreateProgramGL(shaderList);

  glUseProgram(linesProgram);
  matl1 = glGetUniformLocation(linesProgram, "view");
  matl2 = glGetUniformLocation(linesProgram, "proj");

  shaderList = {
    csX75::LoadShaderGL(GL_VERTEX_SHADER, "line_vs.glsl"),
    csX75::LoadShaderGL(GL_FRAGMENT_SHADER, "cursor_fs.glsl")
  };

  cursorProgram = csX75::CreateProgramGL(shaderList);

  glUseProgram(linesProgram);
  matc1 = glGetUniformLocation(linesProgram, "view");
  matc2 = glGetUniformLocation(linesProgram, "proj");
}

void initVertexBufferGL(void)
{
  glGenVertexArrays (1, &cube_vao);
  glGenBuffers (1, &cube_vbo);
  glGenBuffers(1, &cube_ebo);

  set_cube_buf();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo.size()* sizeof(unsigned short), ebo.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray (0);//vertex coordinates
  glEnableVertexAttribArray (1);//colors
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), NULL);
  glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float))); 
  
  //GrayLines
  glGenVertexArrays (1, &lines_vao);
  glBindVertexArray (lines_vao);

  glEnableVertexAttribArray (0);//vertex coordinates

  glGenBuffers (1, &lines_vbo);
  glBindBuffer (GL_ARRAY_BUFFER, lines_vbo);
  glBufferData(GL_ARRAY_BUFFER, linedata.size()*sizeof(glm::vec3), &(linedata[0]), GL_STATIC_DRAW);
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);

  //Cursor
  glGenVertexArrays (1, &cursor_vao);
  glGenBuffers (1, &cursor_vbo);

  set_cursor_buf();
  glEnableVertexAttribArray (0);//vertex coordinates
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
  
  glGenBuffers(1, &cursor_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cursor_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned short), &(ebo[0]), GL_STATIC_DRAW);
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glBindVertexArray (cursor_vao);

  glUseProgram(cursorProgram);
  glUniformMatrix4fv(matc1, 1, GL_FALSE, glm::value_ptr(Vox_cam.view));
  glUniformMatrix4fv(matc2, 1, GL_FALSE, glm::value_ptr(Vox_cam.proj));

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
  
  glBindVertexArray (cube_vao);

  glUseProgram(shaderProgram);
  glUniformMatrix4fv(mat1, 1, GL_FALSE, glm::value_ptr(Vox_cam.view));
  glUniformMatrix4fv(mat2, 1, GL_FALSE, glm::value_ptr(Vox_cam.proj));

  glDrawElements(GL_TRIANGLES, (data.size()*3)/4, GL_UNSIGNED_SHORT, 0);

  glBindVertexArray (lines_vao);

  glUseProgram(linesProgram);
  glUniformMatrix4fv(matl1, 1, GL_FALSE, glm::value_ptr(Vox_cam.view));
  glUniformMatrix4fv(matl2, 1, GL_FALSE, glm::value_ptr(Vox_cam.proj));

  glDrawArrays(GL_LINES, 0, linedata.size());
}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(640, 480, "CS475/CS675 OpenGL Framework", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard and Mouse Callback
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback); 
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
  //Set FPS-type cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Cube_init();
  
  //Initialize GL state
  csX75::initGL();
  initShadersGL();
  initVertexBufferGL();
  
  
  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
  {           
    renderGL();
    // Swap front and back buffers
    glfwSwapBuffers(window);
    
    // Poll for and process events
    curr_frame_time = glfwGetTime();
    delta_time = curr_frame_time - last_frame_time;
    last_frame_time = curr_frame_time;
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

//OTP: 5715