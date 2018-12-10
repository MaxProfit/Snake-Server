namespace snakegame {

class Snake {
  public:
    // Creates a snake using an unoccupied space
    Snake();

    bool IsDead();

    void Update();

    void EatFood();

    int GetFoodEaten();

    direction GetDirection();

    void SetDirection(direction new_direction);


  private:

    // Checks if the head is outside the game space, and the head doesn't hit another snake
    // bool IsDead();

    direction current_direction_;

    int GetFoodEaten();
    void SetDirection(direction new_dir);


};

}