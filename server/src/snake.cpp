//
// Created by Matthew Williams on 2018-12-10.
//

#include "../include/snake.hpp"
#include "../include/board.hpp"
#include "../../lib/json.hpp"

using nlohmann::json;

snakegame::Snake::Snake(int id, snakegameboard::Board& board) : board_(board), id_(id) {

    // Used for generating a random color
    std::random_device rd;
    generator_ = std::mt19937(rd());
    dist_color_ = std::uniform_int_distribution<>(0,255);

    // Create the inital head piece at an empty coord
    std::pair<int, int> head = board_.GetEmptyCoord();

    // Push front of vector by rotating
    coords_.push_back(head);
    std::rotate(coords_.rbegin(), coords_.rbegin() + 1, coords_.rend());

    // Add to the main board for quick lookup
    board_.AddSnakePiece(head);

    // Red
    color_.at(0) = dist_color_(generator_);
    // Green
    color_.at(1) = dist_color_(generator_);
    // Blue
    color_.at(2) = dist_color_(generator_);
}

bool snakegame::Snake::IsAlive(std::pair<int, int> new_head_pos) {
    if (new_head_pos.first < 0
        || new_head_pos.second < 0
        || new_head_pos.first > snakegameboard::kWidth - 1
        || new_head_pos.second > snakegameboard::kHeight - 1
        || board_.IsSnakeHere(new_head_pos)) {

        // Signify this snake needs to die on next update
        alive_ = false;
        score_ = GetFoodEaten();
        // Removes it from the board's representation
        board_.DeleteSnake(id_);
        return false;
    }

    // Snake is still alive
    return true;
}

void snakegame::Snake::Update() {

    std::pair<int, int> one_over;

    switch (current_direction_) {
        case Direction::UP: {
            // Move the snake up one unit on the board (The board is such that top left is 0,0)
            one_over = std::make_pair(coords_.front().first, coords_.front().second - 1);

            break;
        }
        case Direction::DOWN: {
            // Move the snake down one unit on the board (The board is such that top left is 0,0)
            one_over = std::make_pair(coords_.front().first, coords_.front().second + 1);

            break;
        }
        case Direction::LEFT: {
            // Move the snake left one unit on the board (The board is such that top left is 0,0)
            one_over = std::make_pair(coords_.front().first - 1, coords_.front().second);

            break;
        }
        case Direction::RIGHT: {
            // Move the snake right one unit on the board (The board is such that top left is 0,0)
            one_over = std::make_pair(coords_.front().first + 1, coords_.front().second);

            break;
        }
    }

    bool ate_food = false;

    if (board_.IsFoodHere(one_over)) {
        ate_food = true;
        board_.RemoveFood(one_over);
    }

    if (IsAlive(one_over)) {
        // Push front of vector by rotating
        coords_.push_back(one_over);
        std::rotate(coords_.rbegin(), coords_.rbegin() + 1, coords_.rend());

        // Adds this piece's representation to the board
        board_.AddSnakePiece(one_over);
    }

    // If they ate food they don't need the back piece removed
    if (!ate_food) {
        board_.RemoveSnakePiece(coords_.back());
        coords_.pop_back();
    }

    // The user can now update their snake again, to ensure there is no overwriting
    should_update_ = true;
}

void snakegame::Snake::SetDirection(std::string new_dir) {

    if (should_update_) {
        if (new_dir == "W" && current_direction_ != Direction::DOWN && current_direction_ != Direction::UP) {
            current_direction_ = Direction::UP;
        } else if (new_dir == "A" && current_direction_ != Direction::RIGHT && current_direction_ != Direction::LEFT) {
            current_direction_ = Direction::LEFT;
        } else if (new_dir == "S" && current_direction_ != Direction::UP && current_direction_ != Direction::DOWN) {
            current_direction_ = Direction::DOWN;
        } else if (new_dir == "D" && current_direction_ != Direction::LEFT && current_direction_ != Direction::RIGHT) {
            current_direction_ = Direction::RIGHT;
        }
    }
}

int const snakegame::Snake::GetID() const {
    return id_;
}

bool const snakegame::Snake::GetAlive() const {
    return alive_;
}

int const snakegame::Snake::GetFoodEaten() const {
    // Returns the score, which is the length of the snake - 1
    return static_cast<int>(coords_.size() - 1);
}

std::string const snakegame::Snake::GetDirection() const {
    switch (current_direction_) {
        case Direction::UP: {
            return "U";
        }
        case Direction::LEFT: {
            return "L";
        }
        case Direction::DOWN: {
            return "D";
        }
        case Direction::RIGHT: {
            return "R";
        }
    }
}

std::array<int, 3> const snakegame::Snake::GetColor() const {
    return color_;
}

std::vector<std::pair<int, int>> const snakegame::Snake::GetCoords() const {
    return coords_;
}

void snakegame::to_json(nlohmann::json& j, const Snake& s) {
    // This tells the json library how to set up parsing the snake object into a JSON representation
    j = json{{"id", s.GetID()},
             {"length", s.GetFoodEaten()},
             {"alive", s.GetAlive()},
             {"direction", s.GetDirection()},
             {"color", s.GetColor()},
             {"location", s.GetCoords()}
    };
}