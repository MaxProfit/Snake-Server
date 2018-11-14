//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "json.hpp"

using nlohmann::json;
using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::vector<uint8_t> cbor_vec;
typedef std::deque<cbor_vec> cbor_vec_queue;

//----------------------------------------------------------------------

class chat_participant {
public:
  virtual ~chat_participant() {}
  virtual void deliver(const cbor_vec& vec) = 0;
};

// Keeps track of the participants on the server
typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room {
public:
  void join(chat_participant_ptr participant) {
    participants_.insert(participant);
  }

  void leave(chat_participant_ptr participant) {
    participants_.erase(participant);
  }

  void deliver(const cbor_vec& vec) {
    for (auto participant: participants_) {
      participant->deliver(vec);
    }
  }

private:
  std::set<chat_participant_ptr> participants_;
};

//----------------------------------------------------------------------

class chat_session : public chat_participant, public std::enable_shared_from_this<chat_session> {
public:
  chat_session(tcp::socket socket, chat_room& room) : socket_(std::move(socket)), room_(room) {}

  void start() {
    // I should probably have the quick communication here
    room_.join(shared_from_this());
    // do_read_header();
    do_read_json();
  }

  void deliver(const cbor_vec& vec) {
    bool write_in_progress = !write_vecs_.empty();
    write_vecs_.push_back(vec);
    if (!write_in_progress) {
      do_write();
    }
  }

private:

  void do_read_json() {
    auto self(shared_from_this());
    std::cout << "trying to read!" << std::endl;
    std::vector<uint8_t> json_reads (50);
    std::cout << "uhhh" << std::endl;
    boost::system::error_code ec;
    boost::asio::read(socket_, boost::asio::buffer(json_reads), ec);
    if (!ec) {
       json_reads.resize(22);
        std::cout << "no error here~" << std::endl;
        // room_.deliver(json_reads);
        std::cout << "We got here!!" << std::endl;
        json j_from_cbor = json::from_cbor(json_reads);
        std::cout << j_from_cbor["pi"] << std::endl;
        // do read header? 
        // do something with the jsonreads

    } else {
      std::cout << "at the else statement" << std::endl;
      room_.leave(shared_from_this());
    }
       


        
  }

  void do_write() {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(write_vecs_.front()), [this, self](std::error_code ec, std::size_t /*length*/) {
      if (!ec) {
        write_vecs_.pop_front();
        if (!write_vecs_.empty()) {
          do_write();
        }
      } else {
        room_.leave(shared_from_this());
      }
    });
  }

  tcp::socket socket_;
  chat_room& room_;
  cbor_vec read_vec_;
  cbor_vec_queue write_vecs_;

};

//----------------------------------------------------------------------

class chat_server {
public:
  chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint) : acceptor_(io_context, endpoint) {
    do_accept();
  }

private:
  void do_accept() {
    acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
      if (!ec) {
        std::make_shared<chat_session>(std::move(socket), room_)->start();
      }

      do_accept();
    });
  }

  tcp::acceptor acceptor_;
  chat_room room_;
};

//----------------------------------------------------------------------

int main() {
  try {
    boost::asio::io_context io_context;

    std::cout << "at main" << std::endl;

    const unsigned short kPORT {49145};
    tcp::endpoint endpoint(tcp::v4(), kPORT);
    chat_server server(io_context, endpoint);

    io_context.run();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}