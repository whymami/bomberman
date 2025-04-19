#pragma once
#include <iostream>
#include <string>
#include <map>
#include <Player.hpp>

class Map
{
private:
    int _width = 0;
    int _height = 0;
    std::vector<std::string> _map;

public:
    Map(const std::string &path, std::map<std::string, Player *> *playerList);
    ~Map();

    // GETTER
    int get_width();
    int get_height();
    std::vector<std::string> get_map();
    void draw_map();
};