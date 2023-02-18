#ifndef _CUBES_HPP_
#define _CUBES_HPP_

#include<vector>
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

const short n = 20;

bool change_cube_state(short color_ind, bool set);
void update_cursor(short x, short y, short z);
void save_state();
void load_state();
void initState();
void initCubes();

extern std::vector<short> position;
extern short cube_colors[n][n][n];//also keeps track of whether cube is rendered
extern std::vector<glm::vec3> colors;
#endif