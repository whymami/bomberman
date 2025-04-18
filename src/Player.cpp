#include "Player.hpp"

Player::Player(Controller *control, Bomb *bomb, void *player_texture) : control(control), bomb(bomb), player_texture(player_texture)
{
}

Player::~Player()
{
    delete &control;
    delete &bomb;
}

// GETTER
float Player::get_speed()
{
    return _speed;
}

int Player::get_health()
{
    return _health;
}

Bomb *Player::get_bomb()
{
    return bomb;
}

Controller *Player::get_control()
{
    return control;
}

bool Player::get_is_alive()
{
    return _is_alive;
}

// FUNCTION
void Player::increase_speed()
{
    if (_speed < 10)
        _speed += 1;
}
void Player::decrease_speed()
{
    if (_speed > 1)
        _speed -= 1;
}

void Player::increase_health()
{
    if (_health < 10)
        _health += 1;
}

void Player::decrease_health()
{
    if (_health > 0)
        _health -= 1;
    if (_health == 0)
        _is_alive = false;
}