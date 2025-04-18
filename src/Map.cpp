#include <Map.hpp>

Map::Map(char **map, int width, int height) : _width(width), _height(height)
{
}

Map::~Map()
{
}

// GETTER
int Map::get_width()
{
    return _width;
}

int Map::get_height()
{
    return _width;
}

char **Map::get_map()
{
    return _map;
}