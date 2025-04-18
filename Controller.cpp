#include <Controller.hpp>

Controller::Controller(/* args */) {}
Controller::~Controller() {}

// Getter
int Controller::get_up()
{
    return this->_up;
}

int Controller::get_down()
{
    return this->_down;
}

int Controller::get_right()
{
    return this->_right;
}

int Controller::get_left()
{
    return this->_left;
}

int Controller::get_drop_bomb()
{
    return this->_drop_bomb;
}

// Setter
void Controller::set_up(int key)
{
    this->_up = key;
}

void Controller::set_down(int key)
{
    this->_down = key;
}

void Controller::set_right(int key)
{
    this->_right = key;
}

void Controller::set_left(int key)
{
    this->_left = key;
}

void Controller::set_drop_bomb(int key)
{
    this->_drop_bomb = key;
}
