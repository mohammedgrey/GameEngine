#pragma once
#include <iostream>

enum GameState
{
    PLAYING,
    LOST,
    WON,
    PAUSED
};

class Game
{

private:
    int hearts;
    int presentsGoal;
    int presents = 0;
    GameState state = PLAYING;

public:
    Game(int hearts = 3, int presentsGoal = 5)
    {
        this->hearts = hearts;
        this->presentsGoal = presentsGoal;
    };

    void incrementPresents()
    {
        presents++;
        std::cout << "Presents: " << presents << std::endl;
        if (presents == presentsGoal)
            state = WON;
    };
    void decrementHearts()
    {
        hearts--;
        std::cout << "Hearts: " << hearts << std::endl;
        if (hearts <= 0)
            state = LOST;
    };

    ~Game();
};
