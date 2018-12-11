//
// Created by Matthew Williams on 2018-12-10.
//

#ifndef TEST_PROJECT_SNAKE_HPP
#define TEST_PROJECT_SNAKE_HPP

#include <utility>
#include <vector>
#include <deque>
#include "Board.hpp"


namespace snakegame {

    enum class SnakeDirection {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class Snake {
    public:

        // Used to recreate the snake that just died or just connected with the ID
        Snake(int id, snakegame::Board& board);

        // Checks if the Snake is beyond the boundaries of the board, then checks if the snake is hitting another snake
        bool IsAlive();

        // Moves the snake one tile in the direction it's currently heading
        void Update();

        // Increases the snake length by one, by adding onto the tail
        void EatFood();

        // Get the length of the snake - 1, which is equal to the score.
        int GetFoodEaten();

        SnakeDirection GetDirection();

        void SetDirection(std::string new_direction);

        std::deque<std::pair<int, int>> GetCoords() {
            return coords_;
        }


    private:

        // Current direction of the snake
        SnakeDirection current_direction_ = SnakeDirection::RIGHT;

        // A deque to store the coordinates that the snake body resides at
        std::deque<std::pair<int, int>> coords_;

        // The id we refer to this snake by, also links to the client
        int id_;

        // Board that we're able to connect to to get free spaces
        snakegame::Board& board_;

        // Turns false after the user sends a command, until the next tick.
        bool should_update_ = true;
    };

}

#endif //TEST_PROJECT_SNAKE_HPP
