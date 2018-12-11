//
// Created by Matthew Williams on 2018-12-10.
//

#include <bootstrap.h>
#include <random>
#include "Board.hpp"
#include "json.hpp"
#include "Snake.hpp"

using nlohmann::json;

snakegame::Board::Board() {
    generator_ = std::mt19937(rand());
    dist_x_ = std::uniform_int_distribution<>(0, kWidth - 1);
    dist_y_ = std::uniform_int_distribution<>(0, kHeight - 1);

    for (int num_food = 0; num_food < kMaxFood; ++num_food) {
        AddFood(GetEmptyCoord());
    }
}


// Simply gets a random coordinate on the board that is unoccupied
std::pair<int, int> snakegame::Board::GetEmptyCoord() {
    std::pair<int, int> coord;
    do {
        coord = std::make_pair(dist_x_(generator_), dist_y_(generator_));
    } while (IsSnakeHere(coord) || IsFoodHere(coord));

    return coord;
}

void snakegame::Board::DealWithJson(json json_message) {
    int id = json_message.at("id");
    try {
        Snake snake = id_snake_map_.at(id);
        if (json_message.at("action") != "RESET") {
            snake.SetDirection(json_message.at("action"));
        }
    } catch (std::out_of_range& e) {
        Snake(id, this);
    }

}

void snakegame::Board::UpdateBoard() {
    for (std::pair<int, Snake> id_snake : id_snake_map_) {
        id_snake.second.Update();
    }
}

// Checks if there is food at the location the snake is traveling to
bool snakegame::Board::IsFoodHere(std::pair<int, int> location) {
    // Check the board array for food
    return board_array_.at(location.first).at(location.second) == Piece::FOOD;
}

// Checks if there is a snake at the location the snake is traveling to
bool snakegame::Board::IsSnakeHere(std::pair<int, int> location) {
    // Check the board array for snake
    return board_array_.at(location.first).at(location.second) == Piece::SNAKE;
}

// Add food to the board
void snakegame::Board::AddFood(std::pair<int, int> food_coord) {
    // Add food to the board array and the vector of coordinates of food
    board_array_.at(food_coord.first).at(food_coord.second) = Piece::FOOD;
    food_loc_.push_back(food_coord);
}

// Remove food from the board
void snakegame::Board::RemoveFood(std::pair<int, int> food_coord) {
    // Remove food from the board array and the vector of coordinates of food
    // Removing must always cause adding more food
    board_array_.at(food_coord.first).at(food_coord.second) = Piece::EMPTY;
    for (int i = 0; i < food_loc_.size(); ++i) {
        if (food_loc_.at(i) == food_coord) {
            food_loc_.erase(food_loc_.begin() + i);
            break;
        }
    }

    AddFood(GetEmptyCoord());
}

// Add snake piece to the board
void snakegame::Board::AddSnakePiece(std::pair<int, int> snake_piece_coord) {
    // Add a snake piece to the board array
    board_array_.at(snake_piece_coord.first).at(snake_piece_coord.second) = Piece::SNAKE;
}

// Remove snake piece from the board
void snakegame::Board::RemoveSnakePiece(std::pair<int, int> snake_piece_coord) {
    // Remove a snake piece from the board array
    board_array_.at(snake_piece_coord.first).at(snake_piece_coord.second) = Piece::EMPTY;
}

void snakegame::Board::DeleteSnake(int id) {
    std::deque<std::pair<int, int>> snake_coords;
    snake_coords = id_snake_map_.at(id).GetCoords();

    for (std::pair<int, int> pair : snake_coords) {
        RemoveSnakePiece(pair);
    }

    id_snake_map_.erase(id);

}

// TODO: Package the things from the snakes container into an array for JSON
// Package into json for sending, serializes both snakes_loc_ and food_loc_
json snakegame::Board::PackageBoard() {

}
// Sends snakes and food in a coordinate grid JSON


