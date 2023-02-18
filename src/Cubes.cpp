#include "Main.hpp"
#include "Cubes.hpp"

short cube_colors[n][n][n];

float side = 2.0f/n; //size of a side

std::vector<std::vector<short>> Faces;
std::vector<glm::vec3> colors;

std::vector<short> position = {n/2, n/2, n/2};

void addface(short x, short y, short z, short normal, glm::vec3 &color)
{
    //0
    glm::vec3 base = {x*side - 1.0f, y*side - 1.0f, z*side - 1.0f};
    data.push_back(base);
    data.push_back(color);

    //1
    base[(normal + 1)%3] += side;
    data.push_back(base);
    data.push_back(color);

    //2
    base[(normal + 2)%3] += side;
    data.push_back(base);
    data.push_back(color);

    //3
    base[(normal + 1)%3] -= side;
    data.push_back(base);
    data.push_back(color);
}

int findFace(std::vector<short> &Face, int &face_index)
{
    int n = Faces.size();
    for(int i = 0; i < n; i++)
        if((Faces[i][0] == Face[0]) && (Faces[i][1] == Face[1]) && (Faces[i][2] == Face[2]) && (Faces[i][3] == Face[3]))
        {
            face_index = i;
            return true;
        }

    return false;
}

void eraseFace(int m)
{
    Faces[m] = Faces.back();
    Faces.pop_back();

    m*= 8;m+= 7;

    for(int i = 0; i < 8; i++)
    {
        data[m - i] = data.back();
        data.pop_back();
    }
}

bool change_cube_state(short color_ind, bool set)
{
    if(!set &&(cube_colors[position[0]][position[1]][position[2]] == -1))
        return false;
    if(set && (cube_colors[position[0]][position[1]][position[2]] == color_ind))
        return false;

    std::vector<bool> normal(3, false);
    bool further;
    
    for(int i = 0; i < 6; i++)
    {
        normal[i%3] = true;
        further = i/3;
        //i%3 tells normal direction: i = 0 means x and so on
        //i/3 tells whether the face is the one closer to origin (bottom left backward corner)
 
        std::vector<short> Face = {
        (short)(position[0] + (normal[0]&&further)),
        (short)(position[1] + (normal[1]&&further)),
        (short)(position[2] + (normal[2]&&further)), 
        (short)(i%3)};

        int face_index;

        if(findFace(Face, face_index))
        {
            if((cube_colors[position[0]][position[1]][position[2]] == -1) || !set)
            {
                //std::cout << face_index << "\n";
                eraseFace(face_index);
            }
                            
            else
            {
                int m = face_index*8 + 1;
                for(int j = 0; j < 4; j++)
                    data[m + 2*j] = colors[color_ind];
            }
        }

        else
        { 
            Faces.push_back(Face);

            if(cube_colors[position[0]][position[1]][position[2]] == -1)
                addface(Face[0], Face[1], Face[2], Face[3], colors[color_ind]);
            else
            {
                short new_color_ind = cube_colors[position[0] - normal[0] + 2*(normal[0]&&further)]
                                       [position[1] - normal[1] + 2*(normal[1]&&further)]
                                       [position[2] - normal[2] + 2*(normal[2]&&further)];
                
                addface(Face[0], Face[1], Face[2], Face[3], colors[new_color_ind]);
            }
        } 

        normal[i%3] = false;
    }

    //color setting
    if(set)
        cube_colors[position[0]][position[1]][position[2]] = color_ind;
    else
        cube_colors[position[0]][position[1]][position[2]] = -1;

    /*
    int n = Faces.size();
    for(int i = 0; i < n; i ++)
    {
        std::cout << Faces[i][0] << " " << Faces[i][1] << " " << Faces[i][2] << " " << Faces[i][3] << "\n";
    }
    */
   
    //std::cout  << "\n";
    return true;
}

void initState()
{
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            for(int k = 0; k < n; k++)
                cube_colors[i][j][k] = -1;

    colors = {};
    Faces = {};
    data = {};
}

void initCubes()
{
    initState();

    float side_x = -1.0f, side_y;
    for(int i = 0; i < n+1; i++)
    {
        side_y = -1.0f;
        for(int j = 0; j < n+1; j++)
        {
            linedata.push_back({side_x, side_y, -1.0f});
            linedata.push_back({side_x, side_y, 1.0f});
            linedata.push_back({-1.0f, side_x, side_y});
            linedata.push_back({1.0f, side_x, side_y});
            linedata.push_back({side_y, -1.0f, side_x});
            linedata.push_back({side_y, 1.0f, side_x});

            side_y += side;
        }
        side_x += side;
    }

    colors.push_back({0.7f, 0.6f, 1.0f});

    for(int i = 0; i < n*n*n/4; i++)
    {
        unsigned short base_ind = 4*i;
        ebo.insert(ebo.end(),
        {base_ind, (unsigned short)(base_ind + 1),(unsigned short)(base_ind + 2),
        base_ind, (unsigned short)(base_ind + 2), (unsigned short)(base_ind + 3)});
    }
    change_cube_state(0, true);

    for(int i = 0; i < data.size(); i+=2)
        cursordata.push_back(data[i]);

    change_cube_state(0, false);
}

void update_cursor(short x, short y, short z)
{
    position[0] += x;
    if(position[0] < 0 || position[0] >= n)
    {
        position[0] -= x;
        return;
    }

    position[1] += y;
    if(position[1] < 0 || position[1] >= n)
    {
        position[1] -= y;
        return;
    }

    position[2] += z;
    if(position[2] < 0 || position[2] >= n)
    {
        position[2] -= z;
        return;
    }

    for(int i = 0; i < cursordata.size(); i++)
    {
        cursordata[i] += glm::vec3 (side*x, side*y, side*z);
    }
}