#include "../include/chat_client.hpp"

using boost::asio::ip::tcp;

using nlohmann::json;

// Code below derived using https://goo.gl/XHuJZL

chat_client::chat_client(boost::asio::io_context& io_context, 
  const tcp::resolver::results_type& endpoints) 
  : io_context_(io_context), socket_(io_context) {
      do_connect(endpoints);
}

void chat_client::close() {
  boost::asio::post(io_context_, [this]() { socket_.close(); });
}

json chat_client::get_recent_json() {
  return json_recieved_from_server_;
}

void chat_client::send_json(json json_to_send) {
  std::string string_to_send = convert_from_json(json_to_send);
  
  char line[chat_message::max_body_length + 1];

  // Code below from https://goo.gl/q2j48z
  // Copying a cstring with the json contents into a char array for sending
  strncpy(line, string_to_send.c_str(), sizeof(line));
  line[sizeof(line) - 1] = 0;

  // Encode the message and then send it
  chat_message msg;
  msg.body_length(std::strlen(line));
  std::memcpy(msg.body(), line, msg.body_length());
  msg.encode_header();
  write(msg);
}

void chat_client::write(const chat_message& msg) {
  boost::asio::post(io_context_, [this, msg]() {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
        do_write();
    }
  });
}

void chat_client::do_connect(const tcp::resolver::results_type& endpoints) {
  boost::asio::async_connect(socket_, endpoints,
    [this](boost::system::error_code ec, tcp::endpoint) {
      if (!ec) {
        do_read_header();
      }
  });
}

void chat_client::do_read_header() {
  boost::asio::async_read(socket_,
    boost::asio::buffer(read_msg_.data(), chat_message::header_length),
    [this](boost::system::error_code ec, std::size_t /*length*/) {
      if (!ec && read_msg_.decode_header()) {
        do_read_body();
      } else {
        socket_.close();
      }
    });
}

void chat_client::do_read_body() {
  boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          // std::cout.write(read_msg_.body(), read_msg_.body_length());
          // std::cout << "\n";
          json_recieved_from_server_ = convert_to_json(read_msg_.body());
          do_read_header();
        } else {
          socket_.close();
        }
      });
}

void chat_client::do_write() {
  boost::asio::async_write(socket_,
      boost::asio::buffer(write_msgs_.front().data(),
        write_msgs_.front().length()),
      [this](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          write_msgs_.pop_front();
          if (!write_msgs_.empty()) {
            do_write();
          }
        } else {
          socket_.close();
        }
      });
}

json chat_client::convert_to_json(const char* const to_become_json) {
  // Parses the string into json
  std::string std_string(to_become_json);
  return json::parse(std_string);
}

std::string chat_client::convert_from_json(json to_become_string) {
  // Dumps the serialized json to string
  return to_become_string.dump();
}

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
