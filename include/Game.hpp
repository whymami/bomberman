#pragma once
#include <iostream>
#include <string>
#include <map>
#include "Player.hpp"
#include "Map.hpp"

class Game
{
private:
    std::map<std::string, Player *> *_PlayerList = nullptr;
    std::map<std::string, Map *> *_MapList = nullptr;

public:
    Game(/* args */);
    ~Game();

    // GETTER
    std::map<std::string, Player *> *get_player_list();
    std::map<std::string, Map *> *get_map_list();

    void run(int level);
};


