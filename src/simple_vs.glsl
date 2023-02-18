#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 view;
uniform mat4 proj;

out vec3 color;

void main()
{
    gl_Position = proj * view * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    color = aColor;    
}
