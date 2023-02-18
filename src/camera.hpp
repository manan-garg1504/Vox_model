#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum Camera_Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct camera
{
    glm::vec3 cameraPos, target, up, right;
    glm::mat4 view, proj;

    camera();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void Mice(float x, float y, float time);
};

extern camera Vox_cam;

#endif