class Bomb
{
private:
    int _power = 3;
    int _bomb_count = 1;
    int _bomb_dysfunction = 0;
    void *bomb_texture;

public:
    Bomb(/* args */);
    ~Bomb();

    // GETTER
    int get_power();
    int get_bomb_count();
    int get_bomb_dysfunction();

    // SET
    void increase_power();
    void decrease_power();
    void increase_bomb_count();
    void decrease_bomb_count();
    void increase_bomb_dysfunction();
    void decrease_bomb_dysfunction();
};
