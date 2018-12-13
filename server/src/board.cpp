//
// Created by Matthew Williams on 2018-12-10.
//

#include "../include/board.hpp"
#include "../../lib/json.hpp"

using nlohmann::json;

snakegameboard::Board::Board() {

    // Initialize the units of the board_vector_ to empty pieces
    std::array<Piece, kWidth> x_vec;
    std::fill(x_vec.begin(), x_vec.end(), Piece::EMPTY);
    std::fill(board_array_.begin(), board_array_.end(), x_vec);

    // Setup the distributions
    std::random_device rd;
    generator_ = std::mt19937(rd());
    dist_x_ = std::uniform_int_distribution<>(0, kWidth - 1);
    dist_y_ = std::uniform_int_distribution<>(0, kHeight - 1);

    // Generate the food
    for (int num_food = 0; num_food < kMaxFood; ++num_food) {
        AddFood(GetEmptyCoord());
    }
}

// Simply gets a random coordinate on the board that is unoccupied
std::pair<int, int> snakegameboard::Board::GetEmptyCoord() {
    std::pair<int, int> coord;
    do {
        coord = std::make_pair(dist_x_(generator_), dist_y_(generator_));
    } while (IsSnakeHere(coord) || IsFoodHere(coord));

    return coord;
}

void snakegameboard::Board::ConsumeJsonVec(std::vector<json> json_vec) {
    // Do this such that the last thing entered by the user will get processed first
    for (json json_to_parse : json_vec) {
        int id = json_to_parse.at("id");
        try {
            // Checks if the ID is in the map of snakes, if it isn't it creates a snake with that ID
            snakegame::Snake &snake = id_snake_map_.at(id);
            snake.SetDirection(json_to_parse.at("action"));
        } catch (std::out_of_range& e) {
            // If the snake we're trying to reference doesn't exist... just create a new one
            id_snake_map_.insert(std::make_pair(id, snakegame::Snake(id, *this)));
        }
    }
}

void snakegameboard::Board::UpdateBoard() {

    for (auto &id_snake : id_snake_map_) {
        // update all snakes in the map
        // If the snake needs to die, remove it
        if (!id_snake.second.GetAlive()) {
            id_snake_map_.erase(id_snake.second.GetID());
        } else {
            id_snake.second.Update();
        }
    }
}

// Checks if there is food at the location the snake is traveling to
bool snakegameboard::Board::IsFoodHere(std::pair<int, int> location) {
    // Check the board array for food
    try {
        return board_array_.at(location.second).at(location.first) == Piece::FOOD;
    } catch (std::out_of_range& e) {
        return false;
    }
}

// Checks if there is a snake at the location the snake is traveling to
bool snakegameboard::Board::IsSnakeHere(std::pair<int, int> location) {
    // Check the board array for snake
    try {
        return board_array_.at(location.second).at(location.first) == Piece::SNAKE;
    } catch (std::out_of_range& e) {
        return false;
    }
}

// Add food to the board, and to the parseable vector
void snakegameboard::Board::AddFood(std::pair<int, int> food_coord) {
    // Add food to the board array and the vector of coordinates of food
    board_array_.at(food_coord.second).at(food_coord.first) = Piece::FOOD;
    food_loc_.push_back(food_coord);
}

// Remove food from the board
void snakegameboard::Board::RemoveFood(std::pair<int, int> food_coord) {
    // Remove food from the board array and the vector of coordinates of food
    // Removing must always cause adding more food
    board_array_.at(food_coord.second).at(food_coord.first) = Piece::EMPTY;
    for (int index = 0; index < food_loc_.size(); ++index) {
        if (food_loc_.at(index) == food_coord) {
            food_loc_.erase(food_loc_.begin() + index);
            break;
        }
    }

    // Make the food spawn somewhere else, at an empty spot
    AddFood(GetEmptyCoord());
}

// Add snake piece to the board
void snakegameboard::Board::AddSnakePiece(std::pair<int, int> snake_piece_coord) {
    // Add a snake piece to the board array
    board_array_.at(snake_piece_coord.second).at(snake_piece_coord.first) = Piece::SNAKE;
    // Adding to each individual snake is taken care of by the snake class
}

// Remove snake piece from the board
void snakegameboard::Board::RemoveSnakePiece(std::pair<int, int> snake_piece_coord) {
    // Remove a snake piece from the board array
    board_array_.at(snake_piece_coord.second).at(snake_piece_coord.first) = Piece::EMPTY;
    // Removing from the individual snake is taken care of by the snake class
}

void snakegameboard::Board::DeleteSnake(int id) {
    std::vector<std::pair<int, int>> snake_coords;
    snake_coords = id_snake_map_.at(id).GetCoords();

    // Remove each part of the snake from the board array
    for (std::pair<int, int> coord : snake_coords) {
        RemoveSnakePiece(coord);
    }
}

// Package into json for sending, serializes both snakes_loc_ and food_loc_
json snakegameboard::Board::PackageBoard() {
    json j;
    int index = 0;
    // Index is needed for putting the snakes into an array
    for (std::pair<int, snakegame::Snake> pair : id_snake_map_) {
        j["snakes"][index] = pair.second;
        ++index;
    }

    j["food"] = food_loc_;

    return j;
}
