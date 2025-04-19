#pragma once
#include <iostream>
#include <string>

class Map
{
private:
    int _width = 0;
    int _height = 0;
    char **_map = nullptr;

public:
    Map(char **map, int width, int height);
    ~Map();

    // GETTER
    int get_width();
    int get_height();
    char **get_map();
};