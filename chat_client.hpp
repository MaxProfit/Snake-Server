#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "chat_message.hpp"
#include <fstream>
#include "json.hpp"

typedef std::deque<chat_message> chat_message_queue;

// I EVENTUALLY NEED TO PUT A MUTEX LOCK ON READING AND WRITING FILES (json obj)

class chat_client {
    public:
        chat_client(boost::asio::io_context& io_context, 
            const boost::asio::ip::tcp::resolver::results_type& endpoints);

        void write(const chat_message& msg);

        void close();

    private:
        void do_connect(
            const boost::asio::ip::tcp::resolver::results_type& endpoints);

        void do_read_header();

        void do_read_body();

        void do_write();

        boost::asio::io_context& io_context_;
        boost::asio::ip::tcp::socket socket_;
        chat_message read_msg_;
        chat_message_queue write_msgs_;
};

class client_runner {
    public:
        client_runner();
        void set_json(nlohmann::json j);
        nlohmann::json get_json();

    private:
        chat_client client;
        nlohmann::json json_object;
};