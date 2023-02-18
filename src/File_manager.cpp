#include "Cubes.hpp"
#include<iostream>
#include<fstream>

using namespace std;

void save_state()
{
    string s;
    cout << "Enter file name:" << endl;
    cin >> s;
    s = "models/" + s + ".raw";
    ofstream out(s.c_str());

    int x = colors.size();
    out << x << endl;

    for(int i = 0; i < x; i++)
        out << colors[i][0] << " " << colors[i][1] << " " << colors[i][2] << endl;

    x = 0;
    for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
    for(int k = 0; k < n; k++){
        if(cube_colors[i][j][k] != -1)
            x++;
    }
    }
    }

    out << x << endl;

    for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
    for(int k = 0; k < n; k++){
        if(cube_colors[i][j][k] != -1)
            out << i << " " << j << " " << k << " " << cube_colors[i][j][k] << endl;
    }
    }
    }

    out.close();
}

void load_state()
{
    initState();

    string s;
    cout << "Enter file name:" << endl;
    cin >> s;
    s = "models/" + s + ".raw";
    ifstream in(s.c_str());

    if(!in.good())
    {
        cout << "File does not exist" << endl;
        return;
    }

    int x;
    in >> x;

    for(int i = 0; i < x; i++)
    {
        float a, b, c;
        in >> a >> b >> c;
        colors.push_back({a,b,c});
    }

    in >> x;

    short a, b, c, t;

    for(int i = 0; i < x; i++)
    {
        in >> a >> b >> c >> t;

        position = {a,b,c};
        change_cube_state(t, true);
    }

    in.close();
    position = {n/2, n/2, n/2};
}