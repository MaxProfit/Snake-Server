#include "../include/chat_server.hpp"

using nlohmann::json;
using boost::asio::ip::tcp;

int main() {
  try {

    boost::asio::io_context io_context;

    const uint16_t kPORT {49145};
    tcp::endpoint endpoint(tcp::v4(), kPORT);
    chat_server server(io_context, endpoint);

    json json_holder;
    std::ifstream file_input("/Users/matthew/Documents/Xcode/finalproject-MaxProfit/example/nominal_server_to_client.json");
    try {
       file_input >> json_holder;
   } catch (std::exception& e) {
       std::cerr << e.what() << std::endl;
   }

    for (int i = 0; i < 1000; ++i) {
      io_context.poll();
      // server.send_json(json::parse("{ \"happy\": false, \"pi\": 3 }"));
      server.send_json(json_holder);
      sleep(1);
      std::vector<json> json_vec = server.get_json_vector();
      for (json j : json_vec) {
        std::cout << j.dump() << std::endl;
      }
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}