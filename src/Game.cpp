#include "Game.hpp"

Game::Game()
{
    _PlayerList = new std::map<std::string, Player *>();
    _MapList = new std::map<std::string, Map *>();
    for (size_t i = 1; i < 4; i++)
    {
        std::string level = "level-" + std::to_string(i);
        Map *map = new Map(level, _PlayerList);
        _MapList->insert(std::make_pair(level, map));
    }

    for (size_t i = 1; i < 4; i++)
    {
        std::string player_name = "player-" + std::to_string(i);
        Player *player = new Player(nullptr, nullptr, nullptr, 0, 0);
        _PlayerList->insert(std::make_pair(player_name, player));
    }
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

void Game::run(int level)
{
    if (level < 1 || level > 3)
    {
        std::cerr << "Invalid level: " << level << std::endl;
        return;
    }

    std::string level_str = "level-" + std::to_string(level);
    if (_MapList->find(level_str) == _MapList->end())
    {
        std::cerr << "Level not found: " << level_str << std::endl;
        return;
    }

    Map *map = _MapList->at(level_str);
    if (!map)
    {
        std::cerr << "Map not found: " << level_str << std::endl;
        return;
    }
    map->draw_map();
}
