#include "../../lib/json.hpp"
#include "../include/board.hpp"
#include "../include/chat_server.hpp"
#include <boost/asio.hpp>
#include <iostream>

using nlohmann::json;
using boost::asio::ip::tcp;

int main() {
    try {

        // Creates a board
        snakegameboard::Board board;

        boost::asio::io_context io_context;

        // Initializes the server
        const uint16_t kPORT {49145};
        tcp::endpoint endpoint(tcp::v4(), kPORT);
        chat_server server(io_context, endpoint);

        std::chrono::duration<double> elapsed{};

        for (int i = 0; i < 100000; ++i) {
            // Sleep this thread and try and keep the clients in sync with the JSON updates
            std::chrono::milliseconds d = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
            std::this_thread::sleep_for(std::chrono::milliseconds(200) - d);
            // Start a timer for determining how long the next sleep will be
            auto start = std::chrono::high_resolution_clock::now();

            // Checks if the io_context has anything new for us to handle, and deals with it
            io_context.poll();

            // Sends the directions to their appropriate snakes
            board.ConsumeJsonVec(server.get_json_vector());

            // Updates the board logic
            board.UpdateBoard();

            // Sends the new update out to the clients
            json j = board.PackageBoard();
            server.send_json(j);

            auto finish = std::chrono::high_resolution_clock::now();
            elapsed = finish - start;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}