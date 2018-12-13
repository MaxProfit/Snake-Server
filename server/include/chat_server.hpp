#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <fstream>
#include <boost/asio.hpp>
#include <mutex>
#include "../../include/chat_message.hpp"
#include "../../lib/json.hpp"

typedef std::deque<chat_message> message_queue;

class chat_participant {
public:
    virtual ~chat_participant();
    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_room {
public:
    // If a participant joins
    void join(chat_participant_ptr participant);

    // If a participant leaves
    void leave(chat_participant_ptr participant);

    // This connects to every player in the game and delivers a message to them
    void deliver(const chat_message& msg);

    // Sends the json
    void send_json(nlohmann::json json_to_send);

    // Gets a vector of all the pieces of json that all of the clients have sent since we last checked it, then clears
    std::vector<nlohmann::json> get_json_vector();

    // This needs to be called from the read functions
    void add_to_json_vec(nlohmann::json json_to_add);

private:
    std::set<chat_participant_ptr> participants_;
    std::vector<nlohmann::json> json_recieved_vec_;
};

class chat_session :  public chat_participant,
                      public std::enable_shared_from_this<chat_session> {

public:
    chat_session(boost::asio::ip::tcp::socket socket, chat_room& room);
    void start();

    // This is the API we need to connect with to send messages to all players
    void deliver(const chat_message& msg);

private:
    // Reads the header and if there are no integrity issues, read the body
    void do_read_header();

    // Reads the body and puts it into a JSON vector that aggregates from all players
    void do_read_body();

    // Writes to each individual client
    void do_write();

    boost::asio::ip::tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    message_queue write_msgs_;
};

class chat_server {
public:
    chat_server(boost::asio::io_context& io_context,
                const boost::asio::ip::tcp::endpoint& endpoint);

    // Returns a vector of json that were received since the last cycle
    std::vector<nlohmann::json> get_json_vector();

    // Sends a json object to all the participants in the game
    // by calling the chat room's deliver method
    // Sends it to the room object for packaging
    void send_json(nlohmann::json json_to_send);

private:
    // Accept connection from outside world
    void do_accept();

    boost::asio::ip::tcp::acceptor acceptor_;
    chat_room room_;
};