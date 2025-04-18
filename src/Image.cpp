#include "Image.hpp"
#include "stb_image.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Image::Image(std::string path)
{
    int channels;
    unsigned char *data = stbi_load(path.c_str(), &_width, &_height, &channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load image: " << path << std::endl;
    }
    _width = 0;
    _height = 0;
    image_texture = nullptr;
}

Image::~Image()
{
}