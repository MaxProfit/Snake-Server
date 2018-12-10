#include <array>
#include <vector>
#include <utility>
#include <unordered_map>

namespace snakegame {

enum class Piece {
    FOOD,
    SNAKE
};

class Board {
  public:
    // Get the positions of all the different snakes
    // Get the positions of all of the food on the board

    // Simply gets a random coordinate on the board that is unoccupied
    std::pair<int, int> GetEmptyCoord();

    // Initializes the board, adding the food to the board and prepping for connections
    Board();
    // Returns the board for people just starting the game
    std::array<std::array<Piece, 48>, 27> GetBoard();

    // Package into json for sending
    

    // Sends snakes and food in a coordinate grid JSON

  private:
    std::array<std::array<Piece, 1280>, 720> board_;
    std::vector<std::pair<int, int>> food_loc_;
    std::unordered_map<int, std::vector<std::pair<int, int>>> snakes_loc_;
  
};

}