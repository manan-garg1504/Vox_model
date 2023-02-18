#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Cube_init();

#endif