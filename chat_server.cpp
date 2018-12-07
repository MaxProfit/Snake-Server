#include "chat_server.hpp"

using boost::asio::ip::tcp;

using nlohmann::json;

//----------------------------------------------------------------------

chat_participant::~chat_participant() {}

//----------------------------------------------------------------------
// Chat Room

  // Code below derived from https://goo.gl/xuDdLC

void chat_room::join(chat_participant_ptr participant) {
  participants_.insert(participant);
  // Send the init messages to the client, giving them a place or whatever

}

void chat_room::leave(chat_participant_ptr participant) {
  participants_.erase(participant);
}

void chat_room::deliver(const chat_message& msg) {
  for (auto participant: participants_) {
    participant->deliver(msg);
  }
}

#warning TODO: NEEDS TESTING
std::vector<nlohmann::json> chat_room::get_json_vector() {
  std::vector<json> json_vec = json_recieved_vec_;
  // Make sure we don't read the same data twice
  json_recieved_vec_.clear();  
  return json_vec;
}

void chat_room::add_to_json_vec(nlohmann::json json_to_add) {
  json_recieved_vec_.push_back(json_to_add);
}

//------------------------------------------------------------------------------
// Chat Session


chat_session::chat_session(tcp::socket socket, chat_room& room) 
  : socket_(std::move(socket)), room_(room) {}

void chat_session::start() {
  room_.join(shared_from_this());
  do_read_header();
}

void chat_session::deliver(const chat_message& msg) {
  bool write_in_progress = !write_msgs_.empty();
  write_msgs_.push_back(msg);
  if (!write_in_progress) {
    do_write();
  }
}

void chat_session::do_read_header() {
  auto self(shared_from_this());
  boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.data(), chat_message::header_length),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec && read_msg_.decode_header()) {
          do_read_body();
        } else {
          room_.leave(shared_from_this());
          }
      });
}

void chat_session::do_read_body() {
  auto self(shared_from_this());
  boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          #warning TODO: TEST THIS
          // Parses the string into json, then adds it to the vector
          std::string std_string(read_msg_.body());
          room_.add_to_json_vec(json::parse(std_string));


          std::cout << "I received something!!" << std::endl;
          std::cout.write(read_msg_.body(), read_msg_.body_length());
          std::cout << std::endl;
          room_.deliver(read_msg_);
          do_read_header();
        } else {
          room_.leave(shared_from_this());
        }
      });
}

void chat_session::do_write() {
  auto self(shared_from_this());
  boost::asio::async_write(socket_,
      boost::asio::buffer(write_msgs_.front().data(),
        write_msgs_.front().length()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          write_msgs_.pop_front();
          if (!write_msgs_.empty()) {
            do_write();
          }
        } else {
          room_.leave(shared_from_this());
        }
      });
}

//----------------------------------------------------------------------
// Chat Server

chat_server::chat_server( boost::asio::io_context& io_context, 
                          const tcp::endpoint& endpoint) 
                          : acceptor_(io_context, endpoint) {
  do_accept();
}

std::vector<json> chat_server::get_json_vector() {
  return room_.get_json_vector();
}

void chat_server::send_json(json json_to_send) {
  // Convert to a message format and execute
  std::string json_serialize_string = json_to_send.dump();

  char line[chat_message::max_body_length + 1];

  // Code below from https://goo.gl/q2j48z
  // Copying a cstring with the json contents into a char array for sending
  strncpy(line, json_serialize_string.c_str(), sizeof(line));
  line[sizeof(line) - 1] = 0;

  // Encode the message and then send it
  chat_message msg;
  msg.body_length(std::strlen(line));
  std::memcpy(msg.body(), line, msg.body_length());
  msg.encode_header();

  room_.deliver(msg);
}

void chat_server::do_accept() {
  acceptor_.async_accept([this](boost::system::error_code ec,
                                tcp::socket socket) {
    if (!ec) {
      std::make_shared<chat_session>(std::move(socket), room_)->start();
    }

    do_accept();
  });
}

//----------------------------------------------------------------------

// Read from it each time the thing gets updated, but write to it only 100ms
int main() {
  try {

    boost::asio::io_context io_context;

    const uint16_t kPORT {49145};
    tcp::endpoint endpoint(tcp::v4(), kPORT);
    chat_server server(io_context, endpoint);

    // Creates a new thread so we can do other computations while this runs
    std::thread thread([&io_context](){ io_context.run(); });


    // Waits 50 seconds to exit the program
    sleep(5);

    for (json j : server.get_json_vector()) {
      std::cout << j.dump() << std::endl;
    }


    sleep(50);

    thread.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}