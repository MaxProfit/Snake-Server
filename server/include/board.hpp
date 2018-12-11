//
// Created by Matthew Williams on 2018-12-10.
//

#ifndef TEST_PROJECT_BOARD_HPP
#define TEST_PROJECT_BOARD_HPP

#include <array>
#include <vector>
#include <utility>
#include <unordered_map>
#include <random>
#include "json.hpp"
#include "Snake.hpp"

namespace snakegame {

    const uint16_t kMaxFood {5};
    const uint16_t kHeight {27};
    const uint16_t kWidth {48};

    enum class Piece {
        EMPTY,
        FOOD,
        SNAKE
    };

    class Board {
    public:
        // Get the positions of all the different snakes
        // Get the positions of all of the food on the board

        // Initializes the board, adding the food to the board
        Board();
        // Simply gets a random coordinate on the board that is unoccupied
        std::pair<int, int> GetEmptyCoord();

        void DealWithJson(nlohmann::json json_message);

        // Updates all the snakes on the board
        void UpdateBoard();

        // Checks if there is food at the location the snake is traveling to
        bool IsFoodHere(std::pair<int, int> location);

        // Checks if there is a snake at the location the snake is traveling to
        bool IsSnakeHere(std::pair<int, int> location);

        // Add food to the board
        void AddFood(std::pair<int, int> food_coord);

        // Remove food from the board
        void RemoveFood(std::pair<int, int> food_coord);

        // Add snake piece to the board
        void AddSnakePiece(std::pair<int, int> snake_piece_coord);

        // Remove snake piece from the board
        void RemoveSnakePiece(std::pair<int, int> snake_piece_coord);

        // Removes this snake from the board and the vector, used when the snake gets killed
        void DeleteSnake(int id);

        // Package into json for sending, serializes both snakes_loc_ and food_loc_
        nlohmann::json PackageBoard();
        // Sends snakes and food in a coordinate grid JSON

    private:
        std::array<std::array<Piece, kWidth>, kHeight> board_array_;
        std::vector<std::pair<int, int>> food_loc_;
        std::unordered_map<int, snakegame::Snake> id_snake_map_;

        std::mt19937 generator_;
        std::uniform_int_distribution<> dist_x_;
        std::uniform_int_distribution<> dist_y_;

    };
}


#endif //TEST_PROJECT_BOARD_HPP
