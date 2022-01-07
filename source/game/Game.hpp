#pragma once
#include <iostream>

enum GameState
{
    PLAYING,
    LOST,
    WON,
    PAUSED
};
namespace our
{
    class Game
    {

    private:
        int hearts;
        int presentsGoal;
        int presents = 0;

    public:
        GameState state = PLAYING;
        Game(int hearts = 3, int presentsGoal = 5)
        {
            this->hearts = hearts;
            this->presentsGoal = presentsGoal;
        };
        void resetGame()
        {
            this->hearts = 3;
            this->presents = 0;
            this->presentsGoal = 5;
            state = PLAYING;
        }

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

        ~Game()
        {
            resetGame();
        }
    };
}
