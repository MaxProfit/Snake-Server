//
// chat_client.cpp
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
#include <thread>
#include <boost/asio.hpp>
#include <vector>
#include "json.hpp"
#include <cstdlib>

// For ease of use
using boost::asio::ip::tcp;
using nlohmann::json;

typedef std::vector<uint8_t> cbor_vec;
typedef std::deque<cbor_vec> cbor_vec_queue;


class chat_client {
public:
  chat_client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints) : io_context_(io_context), socket_(io_context) {
    do_connect(endpoints);
  }

  void write(const cbor_vec& vec) {
    boost::asio::post(io_context_, [this, vec]() {
      bool write_in_progress = !write_vecs_.empty();
      write_vecs_.push_back(vec);
      if (!write_in_progress) {
        do_write_vector();
      }
    });
  }

  // When you close the client in the main, shut down the sockets
  void close() {
    boost::asio::post(io_context_, [this]() { socket_.close(); });
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints) {
    std::cout << "trying to connect!!" << std::endl;
    boost::asio::async_connect(socket_, endpoints, [this](std::error_code ec, tcp::endpoint) {
      if (!ec) {
        // On connection to server, what it does
        std::cout << "I'm in do_connect!" << std::endl;
        // do_read_vector();
      }
    });
  }

  void do_read_vector() {
    // Simply reads into the buffer
    boost::system::error_code ec;
    std::cout << "I'm starting to read a vector!" << std::endl;
    boost::asio::read(socket_, boost::asio::buffer(read_vec_), ec);
    if (!ec) {
      std::cout << read_vec_.at(0) << std::endl;
      json j_from_cbor = json::from_cbor(read_vec_);
      std::cout <<  j_from_cbor["pi"] << std::endl;
    } else {
      socket_.close();
    }
  }

  void do_write_vector() {
    boost::system::error_code ec;
    boost::asio::write(socket_, boost::asio::buffer(write_vecs_.front()), ec);
    if (!ec) {
      // This keeps going until the write queue is empty
      write_vecs_.pop_front();
      if (!write_vecs_.empty()) {
        do_write_vector();
      }
    } else {
      socket_.close();
    }
  }

private:
  boost::asio::io_context& io_context_;
  tcp::socket socket_;
  cbor_vec read_vec_;
  cbor_vec_queue write_vecs_;
};

int main() {
  try {
    boost::asio::io_context io_context;

    const std::string kPORT ("49145");
    const std::string kADDRESS ("54.237.158.244");
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(kADDRESS.c_str(), kPORT.c_str());
    chat_client client(io_context, endpoints);

    // std::thread thread([&io_context](){ io_context.run(); });

   

    json j;
	  j["pi"] = 3.1415;
	  j["list"] = {1,2,3};
	  std::vector<std::uint8_t> v_cbor = json::to_cbor(j);
    client.write(v_cbor);

    std::cout << v_cbor.size() << std::endl;

    sleep(5);

    client.close();
    // thread.join();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}