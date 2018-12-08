#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <fstream>
#include "chat_message.hpp"
#include "json.hpp"

typedef std::deque<chat_message> chat_message_queue;

class chat_client {
  public:
    chat_client(boost::asio::io_context& io_context, 
        const boost::asio::ip::tcp::resolver::results_type& endpoints);

    void close();

    // Returns the server's most recent json packet
    nlohmann::json get_recent_json();

    // Invokes the sending function to send the packet to the server
    void send_json(nlohmann::json json_to_send);

  private:
    void write(const chat_message& msg);

    void do_connect(
        const boost::asio::ip::tcp::resolver::results_type& endpoints);

    void do_read_header();

    void do_read_body();

    void do_write();

    // Converts from c string to json object
    nlohmann::json convert_to_json(const char* const to_become_json);

    // Converts from json to a c string
    std::string convert_from_json(nlohmann::json to_become_string);

    nlohmann::json json_recieved_from_server_;
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};