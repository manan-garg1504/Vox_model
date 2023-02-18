#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <string>
#include <iostream>
#include <vector>
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

extern std::vector<glm::vec3> data, linedata, cursordata;
extern std::vector<unsigned short> ebo;
extern unsigned int cube_vao, cube_vbo, cube_ebo, lines_vao, lines_vbo, cursor_vao, cursor_vbo, cursor_ebo;
extern float curr_frame_time, last_frame_time, delta_time;

void set_cube_buf();
void set_cursor_buf();

#endif