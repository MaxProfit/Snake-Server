namespace snakegame {

class Snake {
  public:
    // Creates a snake using an unoccupied space
    Snake();

  private:

    // Checks if the head is outside the game space, and the head doesn't hit another snake
    bool IsDead();

};

}