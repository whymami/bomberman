#pragma once
#include <iostream>
#include <string>

class Map
{
private:
    int _width = 0;
    int _height = 0;

public:
    Map(/* args */);
    ~Map();

    // GETTER
    int get_width();
    int get_height();

    // SETTER
    void set_width(int width);
    void set_height(int height);

    // FUNCTION
    void load_map(std::string path);
};