#include "../../include/json.hpp"
#include <iostream>
#include <fstream>

using nlohmann::json;

json receive_json() {
    // Open the file and read into memory
    json json_holder;
    std::ifstream file_input("/Users/matthew/Documents/Xcode/finalproject-MaxProfit/interchange/to_server.json");
    // Feed in the scoresheet to the json object
    file_input >> json_holder;
    
    return json_holder;
}

void send_json(json json_to_send) {
    std::ofstream file_output("/Users/matthew/Documents/Xcode/finalproject-MaxProfit/interchange/to_client.json");
    // Pushes the json to the file with a width of
    file_output << std::setw(2) << json_holder << std::endl;
}

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