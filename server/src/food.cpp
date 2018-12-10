#include "../include/food.hpp"

SnakeFood::SnakeFood() {
    generator_ = std::mt19937(rand());
    dist_x_ = std::uniform_int_distribution<>(0, snakegame::kWidth);
    dist_y_ = std::uniform_int_distribution<>(0, snakegame::kHeight);

    rebase();
}

void SnakeFood::rebase() {
    auto x = dist_x_(generator_);
    auto y = dist_y_(generator_);
    // Set position here
}