#pragma once
#include <iostream>

enum GameState
{
    PLAYING,
    LOST,
    WON,
    PAUSED // added in case a pause state is going to be there but till now it's not used
};
namespace our
{
    // This class is responsible for handling the game logic and keeping track of all the states and changes
    //  that are happening in the game
    class Game
    {

    private:
        int hearts;       // keep track of the hearts I have
        int presentsGoal; // The Number Goal of the presents I have to collect
        int presents = 0; // keep track of the number of presents I have collected

    public:
        GameState state = PLAYING; // The initial state of the game is playing

        // Initialize the game states
        Game(int hearts = 3, int presentsGoal = 5)
        {
            this->hearts = hearts;
            this->presentsGoal = presentsGoal;
        };

        // For reseting the game to its initial state
        void resetGame()
        {
            this->hearts = 3;
            this->presents = 0;
            this->presentsGoal = 5;
            state = PLAYING;
        }

        // For incrementing the presents
        void incrementPresents()
        {
            presents++;
            std::cout << "Presents: " << presents << std::endl;
            if (presents == presentsGoal)
                state = WON; // Change the state if I have reached the winning goal
        };

        // For decrementing the hearts
        void decrementHearts()
        {
            hearts--;
            std::cout << "Hearts: " << hearts << std::endl;
            if (hearts <= 0)
                state = LOST; // Change the state if I have lost all my hearts
        };

        ~Game()
        {
            resetGame();
        }
    };
}
