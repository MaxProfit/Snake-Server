//
// Created by Matthew Williams on 2018-12-10.
//

#ifndef TEST_PROJECT_SNAKE_HPP
#define TEST_PROJECT_SNAKE_HPP

// Forward definition needed for compiler to not be angry
namespace snakegameboard {
    class Board;
};

namespace snakegame {

    // Directions that the snake can be in
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    // Forward declaration needed for the compiler
    class Snake;

    // Transforms the snake object into a json object by specifying which fields go where
    void to_json(nlohmann::json& j, const snakegame::Snake& s);

    class Snake {
    public:

        // Used to recreate the snake that just died or just connected with the ID, also generates a color
        Snake(int id, snakegameboard::Board& board);

        // Checks if the Snake is beyond the boundaries of the board, then checks if the snake is hitting another snake
        bool IsAlive(std::pair<int, int> new_head_pos);

        // Moves the snake one tile in the direction it's currently pointed towards
        void Update();

        // Sets the direction of the snake, comes from the json, given in WASD because buffers are weird
        void SetDirection(std::string new_dir);

        // Returns the ID of the snake
        int const GetID() const;

        // Get the length of the snake - 1, which is equal to the score.
        int const GetFoodEaten() const;

        // Gets the alive_ field, which is used to give the JSON extraction the correct values
        bool const GetAlive() const;

        // Returns the current direction the snake is heading as a string
        std::string const GetDirection() const;

        // Returns the RGB value of the snake's color
        std::array<int, 3> const GetColor() const;

        // Gets the vector of coordinates the snake is currently occupying
        std::vector<std::pair<int, int>> const GetCoords() const;

    private:
        // Used to generate the colors
        std::mt19937 generator_;
        std::uniform_int_distribution<> dist_color_;

        // The score
        int score_;

        // Current direction of the snake, starting off as right
        Direction current_direction_ = Direction::RIGHT;

        // A deque to store the coordinates that the snake body resides at
        std::vector<std::pair<int, int>> coords_;

        // The id we refer to this snake by, also links to the client
        int id_;

        // Board that we're able to connect to to get free spaces
        snakegameboard::Board& board_;

        // Turns false after the user sends a command, until the next tick.
        bool should_update_ = true;

        // Color array
        std::array<int, 3> color_;

        // Stores the alive
        bool alive_ = true;
    };
}

#endif //TEST_PROJECT_SNAKE_HPP
