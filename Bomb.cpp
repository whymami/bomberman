#include <Bomb.hpp>

Bomb::Bomb(/* args */)
{
}

Bomb::~Bomb()
{
}

// GETTER
int Bomb::get_power()
{
    return _power;
}

int Bomb::get_bomb_count()
{
    return _bomb_count;
}

int Bomb::get_bomb_dysfunction()
{
    return _bomb_dysfunction;
}

// SET
void Bomb::increase_power()
{
    if (_power < 10)
        _power += 1;
}

void Bomb::decrease_power()
{
    if (_power > 1)
        _power -= 1;
}

void Bomb::increase_bomb_count()
{
    if (_bomb_count < 10)
        _bomb_count += 1;
}

void Bomb::decrease_bomb_count()
{
    if (_bomb_count > 0)
        _bomb_count -= 1;
}

void Bomb::increase_bomb_dysfunction()
{
    if (_bomb_dysfunction < 10)
        _bomb_dysfunction += 1;
}

void Bomb::decrease_bomb_dysfunction()
{
    if (_bomb_dysfunction > 0)
        _bomb_dysfunction -= 1;
}