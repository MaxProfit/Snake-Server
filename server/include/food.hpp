#include <utility>
#include <random>


namespace snakegame {

class Food {

  public:
    // Generates food over a distribution
    Food();
    void rebase();
    std::pair<int,int> GetLocation();

  private:
    int grid_height_;
    int grid_width_;
    std::mt19937 generator_;
    std::uniform_int_distribution<> dist_width_;
    std::uniform_int_distribution<> dist_height_;
};

}