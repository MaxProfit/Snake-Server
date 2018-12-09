#include "../include/chat_server.hpp"

using nlohmann::json;
using boost::asio::ip::tcp;

// int main() {
//     try {

//         boost::asio::io_context io_context;

//         // This work object keeps the thread 
//         // alive and not quitting on us prematurely
//         boost::asio::io_context::work work(io_context);

//         const uint16_t kPORT {49145};
//         tcp::endpoint endpoint(tcp::v4(), kPORT);
//         chat_server server(io_context, endpoint);

//         // Creates a new thread so we can do other computations while this runs
//         std::thread thread([&io_context](){ io_context.run(); });

//         // The code will continue to run forever because the server always waiting
//         // for more connections, it will not stop until the program is exited
//         sleep(4);

//         server.send_json(json::parse("{ \"happy\": nah, \"pi\": 3 }"));

//         std::cout << server.get_json_vector().at(0).dump() << std::endl;

//         sleep(4);

//         // thread.join();
//         thread.detach();

//     } catch (std::exception& e) {
//         std::cerr << "Exception: " << e.what() << "\n";
//     }

//     return 0;
// }

int main() {
  try {

    boost::asio::io_context io_context;

    // This work object keeps the thread 
    // alive and not quitting on us prematurely
    boost::asio::io_context::work work(io_context);

    const uint16_t kPORT {49145};
    tcp::endpoint endpoint(tcp::v4(), kPORT);
    chat_server server(io_context, endpoint);

    // Creates a new thread so we can do other computations while this runs
    std::thread thread([&io_context](){ io_context.run(); });

    // The code will continue to run forever because the server always waiting
    // for more connections, it will not stop until the program is exited
    sleep(4);

    server.send_json(json::parse("{ \"happy\": nah, \"pi\": 3 }"));

    thread.join();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}