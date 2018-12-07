#include "chat_server.hpp"

using boost::asio::ip::tcp;

using nlohmann::json;

//----------------------------------------------------------------------

chat_participant::~chat_participant() {}

//----------------------------------------------------------------------

  // Code below derived from https://goo.gl/xuDdLC

void chat_room::join(chat_participant_ptr participant) {
  participants_.insert(participant);
  for (auto msg: recent_msgs_) {
    participant->deliver(msg);
  }
}

void chat_room::leave(chat_participant_ptr participant) {
  participants_.erase(participant);
}

void chat_room::deliver(const chat_message& msg) {
  recent_msgs_.push_back(msg);
  while (recent_msgs_.size() > max_recent_msgs) {
    recent_msgs_.pop_front();
  }  

  for (auto participant: participants_) {
    participant->deliver(msg);
  }
}

//------------------------------------------------------------------------------

chat_session::chat_session(tcp::socket socket, chat_room& room) : socket_(std::move(socket)), room_(room) {}

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
          std::cout << "I received something!!" << std::endl;
          std::ofstream o("pretty.json");
          o.write(read_msg_.body(), read_msg_.body_length());
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

chat_server::chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint) : acceptor_(io_context, endpoint) {
  do_accept();
}

void chat_server::do_accept() {
  acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
    if (!ec) {
      std::make_shared<chat_session>(std::move(socket), room_)->start();
    }

    do_accept();
  });
}

//----------------------------------------------------------------------

// Read from it each time the thing gets updated, but write to it only every 100ms
int main() {
  try {
  

    boost::asio::io_context io_context;

    const uint16_t kPORT {49145};
    tcp::endpoint endpoint(tcp::v4(), kPORT);
    chat_server server(io_context, endpoint);

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}