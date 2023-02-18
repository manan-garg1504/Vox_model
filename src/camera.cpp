#include "camera.hpp"
#include <iostream>

const float MovementSpeed = 10.0f;
void camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    glm::vec3 change = target * MovementSpeed * deltaTime;
    if (direction == FORWARD)
        cameraPos += change;
    if (direction == BACKWARD)
        cameraPos -= change;

    change = right * MovementSpeed * deltaTime;
    if (direction == LEFT)
        cameraPos -= change;
    if (direction == RIGHT)
        cameraPos += change;

    view = glm::lookAt(cameraPos, target + cameraPos, up);
}

void camera::Mice(float x, float y, float time)
{
    if(x == 0 && y ==0)
        return;
    
    x*=time; y*=-time; //parity fixing in y
    glm::vec3 input_vec = glm::normalize(x*right + y*up);
    glm::vec3 axis = glm::cross(input_vec, target);
    
    glm::quat rot = glm::angleAxis(float(sqrt(x*x + y*y)), axis);
    glm::mat4 rotMatrix = glm::mat4_cast(rot);

    glm::vec4 target_new = rotMatrix*glm::vec4(target, 1.0f);
    glm::vec4 up_new = rotMatrix*glm::vec4(up, 1.0f);

    target = glm::vec3(target_new.x, target_new.y, target_new.z);
    up = glm::vec3(up_new.x, up_new.y, up_new.z);
    right = glm::cross(target, up);

    view = glm::lookAt(cameraPos, target + cameraPos, up);
}

camera::camera()
{
    cameraPos = {0.0f, 0.0f, 3.5f};//can be modified by rotation matrices
    target = {0.0f, 0.0f, -1.0f};
    up = {0.0f, 1.0f, 0.0f};// useful for rotation: rotate around this for right-left; 
    right = glm::cross(target, up);
    
    view = glm::lookAt(cameraPos, target + cameraPos, up);
    proj = glm::perspective(glm::radians(45.0f), 8.0f/6.0f, 0.5f, 10.0f);
}