#include "Map.hpp"
#include <utils/json_loader.hpp>

Map::Map(const std::string &level, std::map<std::string, Player *> *playerList)
{
    std::string path = "levels/" + level + ".json";

    nlohmann::json json = load_json(path);
    _width = json["width"];
    _height = json["height"];
    _map = json["map"].get<std::vector<std::string>>();
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

std::vector<std::string> Map::get_map()
{
    return _map;
}

void Map::draw_map()
{
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            if (_map[i][j] == '1')
                std::cout
                    << "█";
            else if (_map[i][j] == '0')
                std::cout
                    << " ";
            else
                std::cout << _map[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "Width: " << _width << ", Height: " << _height << std::endl;
}