#include "Input.hpp"
#include "Main.hpp"
#include "Cubes.hpp"
#include "camera.hpp"

int curr_col_index = 0;

void Cube_init()
{
    initCubes();
}

//trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f));
//sender:
int get_col_index(float R, float G, float B)
{
    glm::vec3 color = {R, G, B};

    int n = colors.size(), index = n;
    for(int i = 0; i < n; i++)
        if(colors[i] == color)
            index = i;

    if(index == n)
        colors.push_back(color);

    return index;
}
float get_col()
{
    float col;

    while (true)
    {
        std::cin >> col;
        if(col<= 1.0f && col >= 0.0f)
            break;
        
        std::cout << "Value should be between 0 and 1\n";
    }

    return col;
}

bool dir_press[6] = {false, false, false, false, false, false};
bool cam_dir[4] = {false, false, false, false};
bool set_press = false, reset_press = false, color_set = false, save = false, load = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        if(!dir_press[0])
        {
            update_cursor(-1,0,0);
            set_cursor_buf();
        }        
        dir_press[0] = true;
    }
    else
        dir_press[0] = false;

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        if(!dir_press[1])
        {
            update_cursor(1,0,0);
            set_cursor_buf();
        }        
        dir_press[1] = true;
    }
    else
        dir_press[1] = false;
    
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        if(!dir_press[2])
        {
            update_cursor(0,0,-1);
            set_cursor_buf();
        }        
        dir_press[2] = true;
    }
    else
        dir_press[2] = false;
    
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        if(!dir_press[3])
        {
            update_cursor(0,0,1);
            set_cursor_buf();
        }        
        dir_press[3] = true;
    }
    else
        dir_press[3] = false;

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        if(!dir_press[4])
        {
            update_cursor(0,1,0);
            set_cursor_buf();
        }        
        dir_press[4] = true;
    }
    else
        dir_press[4] = false;

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        if(!dir_press[5])
        {
            update_cursor(0,-1,0);
            set_cursor_buf();
        }        
        dir_press[5] = true;
    }
    else
        dir_press[5] = false;

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if(!set_press)
        {
            if(change_cube_state(curr_col_index, true))
                set_cube_buf();
        }
        set_press = true;
    }
    else
        set_press = false;

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if(!reset_press)
        {
            if(change_cube_state(curr_col_index, false))
                set_cube_buf();
        }
        reset_press = true;
    }
    else
        reset_press = false;

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        if(!color_set)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            std::cout << "Enter RGB values for new color\n";

            std::cout << "Enter red\n";
            float red = get_col();

            std::cout << "Enter green\n";
            float green = get_col();

            std::cout << "Enter blue\n";
            float blue = get_col();

            curr_col_index = get_col_index(red,green,blue);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        color_set = true;
    }
    else
        color_set = false;

    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        if(!save)
            save_state();
        save = true;
    }
    else
        save = false;

    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        if(!load)
        {
            load_state();
            set_cube_buf();
            set_cursor_buf();

            curr_col_index = colors.size() - 1;
        }
        load = true;
    }
    else
        load = false;

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        cam_dir[0] = true;
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        cam_dir[0] = false;

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        cam_dir[1] = true;
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        cam_dir[1] = false;

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        cam_dir[2] = true;
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        cam_dir[2] = false;

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        cam_dir[3] = true;
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        cam_dir[3] = false;

    if(cam_dir[0])
        Vox_cam.ProcessKeyboard(FORWARD, delta_time);
    if(cam_dir[1])
        Vox_cam.ProcessKeyboard(BACKWARD, delta_time);
    if(cam_dir[2])
        Vox_cam.ProcessKeyboard(LEFT, delta_time);
    if(cam_dir[3])
        Vox_cam.ProcessKeyboard(RIGHT, delta_time);
}

bool firstMouse = true;
double prev_X, prev_Y;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        prev_X = xpos;
        prev_Y = ypos;
        firstMouse = false;
    }
  
    Vox_cam.Mice(xpos - prev_X, ypos - prev_Y, delta_time);
    prev_X = xpos;
    prev_Y = ypos;
}