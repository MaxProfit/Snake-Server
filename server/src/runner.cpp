int main() {
    // Init server

    // Create food map

    // Ready to recieve snakes

    // Game loop

    while (1) {
        // Get the latest info on the snakes

        // run through the game loop
        // Check every snake to see if they changed direction
        // Check if they're dead, if they reset their game

        // Check if the local snakes have eaten any food
        

        // Run through all the snakes
        // if we should update and
        // if the snake isn't dead

        for (snake : snakes_) {
            if (should_update_ && current_state_ = IN_PROGRESS) {

                if (snake_body_.intersects(food)) {
                    // Eat the food
                    // Make the food respawn
                }

                game_snake.update(); // updates the snake one square in the current direction
                
                if (snake_body_.intersects(all snakes)) {
                    current_state_ = FINISHED
                }

            } 

            should_update_ = true;
        }


        // MAKE THE JSON FILE THAT THE TIMER WILL PULL FROM
    }

    // Create timer to update the game every 1/20th of a second, sending the
    // new info to the clients
}