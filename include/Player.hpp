#pragma once 
#include "Controller.hpp"
#include "Bomb.hpp"

class Player
{
private:
    bool _is_alive = true;
    float _speed = 1;
    int _health = 3;
    Bomb *bomb;
    Controller *control;
    void *player_texture;
    int x = 1;
    int y = 1;

public:
    Player(Controller *control, Bomb *bomb, void *player_texture, int x, int y);
    ~Player();

    // GETTER
    float get_speed();
    int get_health();
    Bomb *get_bomb();
    Controller *get_control();
    bool get_is_alive();

    // FUNCTION
    void increase_speed();
    void decrease_speed();
    void increase_health();
    void decrease_health();
};
