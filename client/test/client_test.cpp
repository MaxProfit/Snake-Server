#include "../include/chat_client.hpp"

using boost::asio::ip::tcp;
using nlohmann::json;

// int main() {
//     try {

//         boost::asio::io_context io_context;

//         const std::string kADDRESS("54.237.158.244");
//         const std::string kPORT("49145");
//         tcp::resolver resolver(io_context);
//         auto endpoints = resolver.resolve(kADDRESS.c_str(), kPORT.c_str());
//         chat_client c(io_context, endpoints);

//         std::thread t([&io_context](){ io_context.run(); });
  
//         auto j3 = json::parse("{ \"happy\": true, \"pi\": 3.141 }");
//         c.send_json(j3);

//         sleep(5);

//         std::cout << c.get_recent_json().dump() << std::endl;

//         c.close();
//         t.join();

//     } catch (std::exception& e) {
//         std::cerr << "Exception: " << e.what() << "\n";
//     }

//     return 0;
// }

int main() {
  try {
    boost::asio::io_context io_context;

    const std::string kADDRESS("54.237.158.244");
    const std::string kPORT("49145");
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(kADDRESS.c_str(), kPORT.c_str());
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });
  
    auto j3 = json::parse("{ \"happy\": true, \"pi\": 3.141 }");
    c.send_json(j3);

    sleep(5);

    std::cout << c.get_recent_json().dump() << std::endl;

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}