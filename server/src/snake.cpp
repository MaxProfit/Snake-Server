//
// Created by Matthew Williams on 2018-12-10.
//

#include "Snake.hpp"
#include "Board.hpp"

// TODO: instantiate the board object reference
snakegame::Snake::Snake(int id, snakegame::Board& board) : board_(board), id_(id) {
    coords_.push_front(board_.GetEmptyCoord());
}

void snakegame::Snake::Update() {

    switch (current_direction_) {
        case SnakeDirection::UP: {
            std::pair<int, int> up_one(coords_.at(0).first, coords_.at(0).second - 1);
            coords_.push_front(up_one);
            board_.AddSnakePiece(up_one);
            break;
        }
        case SnakeDirection::DOWN: {
            std::pair<int, int> down_one(coords_.at(0).first, coords_.at(0).second + 1);
            coords_.push_front(down_one);
            board_.AddSnakePiece(down_one);
            break;
        }
        case SnakeDirection::LEFT: {
            std::pair<int, int> left_one(coords_.at(0).first - 1, coords_.at(0).second);
            coords_.push_front(left_one);
            board_.AddSnakePiece(left_one);
            break;
        }
        case SnakeDirection::RIGHT: {
            std::pair<int, int> right_one(coords_.at(0).first + 1, coords_.at(0).second);
            coords_.push_front(right_one);
            board_.AddSnakePiece(right_one);
            break;
        }
    }

    coords_.pop_back();

    should_update_ = true;
}

bool snakegame::Snake::IsAlive() {
    if (coords_.front().first < 0
        || coords_.front().second < 0
        || coords_.front().first > kWidth - 1
        || coords_.front().second > kHeight - 1) {

        return false;
    } else if (board_.IsSnakeHere(coords_.front())) {
        return false;
    }

    // Uses the board to rid the existence of this snake
    board_.DeleteSnake(id_);
    return true;

    // Eventually this needs to cause the snake to be erased from the board
}

void snakegame::Snake::EatFood() {
    std::pair<int, int> food_location(coords_.back().first, coords_.back().second);
    coords_.push_back(food_location);
    board_.AddSnakePiece(food_location);
    // Eat it from the board
    board_.RemoveFood(food_location);

}

int snakegame::Snake::GetFoodEaten() {
    return static_cast<int>(coords_.size() - 1);
}

snakegame::SnakeDirection snakegame::Snake::GetDirection() {
    return current_direction_;
}

void snakegame::Snake::SetDirection(std::string new_direction) {
    if (should_update_) {
        if (new_direction == "UP" && current_direction_ != SnakeDirection::DOWN && current_direction_ != SnakeDirection::UP) {
            current_direction_ = SnakeDirection::UP;
            should_update_ = false;
        } else if (new_direction == "LEFT" && current_direction_ != SnakeDirection::RIGHT && current_direction_ != SnakeDirection::LEFT) {
            current_direction_ = SnakeDirection::LEFT;
            should_update_ = false;
        } else if (new_direction == "DOWN" && current_direction_ != SnakeDirection::UP && current_direction_ != SnakeDirection::DOWN) {
            current_direction_ = SnakeDirection::DOWN;
            should_update_ = false;
        } else if (new_direction == "RIGHT" && current_direction_ != SnakeDirection::LEFT && current_direction_ != SnakeDirection::RIGHT) {
            current_direction_ = SnakeDirection::RIGHT;
            should_update_ = false;
        }
    }

}