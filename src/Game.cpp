#include "Game.hpp"

Game::Game(/* args */)
{
    _PlayerList = new std::map<std::string, Player *>();
    _MapList = new std::map<std::string, Map *>();
}

Game::~Game()
{
    for (auto &player : *_PlayerList)
        if (player.second)
        {
            delete player.second;
            player.second = nullptr;
        }
    delete _PlayerList;

    for (auto &map : *_MapList)
        if (map.second)
        {
            delete map.second;
            map.second = nullptr;
        }
    delete _MapList;
}

std::map<std::string, Player *> *Game::get_player_list()
{
    return _PlayerList;
}
std::map<std::string, Map *> *Game::get_map_list()
{
    return _MapList;
}