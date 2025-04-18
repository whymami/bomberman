#pragma once
#include <iostream>
#include <string>

class Image
{
private:
    int _width = 0;
    int _height = 0;
    void *image_texture = nullptr;

public:
    Image(std::string path);
    ~Image();
};
