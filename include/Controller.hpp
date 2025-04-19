#pragma once 

#include <iostream>
#include <string>

class Controller
{
private:
    int _up;
    int _down;
    int _right;
    int _left;
    int _drop_bomb;

public:
    Controller(/* args */);
    ~Controller();
    
    //Getter
    int get_up();
    int get_down();
    int get_right();
    int get_left();
    int get_drop_bomb();

    //Setter
    void set_up(int key);
    void set_down(int key);
    void set_right(int key);
    void set_left(int key);
    void set_drop_bomb(int key);
};
