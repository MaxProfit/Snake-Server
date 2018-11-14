using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  chat_client(boost::asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    boost::asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    boost::asio::post(io_context_, [this]() { socket_.close(); });
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    boost::asio::async_connect(socket_, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_read_body()
  {
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_write()
  {
    boost::asio::async_write(socket_,
        boost::asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }

private:
  boost::asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

int main()
{
  try
  {
    boost::asio::io_context io_context;

    const std::string kADDRESS("54.237.158.244");
    const std::string kPORT("49145");
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(kADDRESS.c_str(), kPORT.c_str());
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });
    

    std::ifstream i("init_client_to_server.json");
    std::string str;
    std::string file_contents;
    while (std::getline(i, str))
    {
      file_contents += str;
      file_contents.push_back('\n');
    }

    char line[chat_message::max_body_length + 1];

    // Code below from https://stackoverflow.com/questions/13294067/how-to-convert-string-to-char-array-in-c
    strncpy(line, file_contents.c_str(), sizeof(line));
    line[sizeof(line) - 1] = 0;

    chat_message msg;
    msg.body_length(std::strlen(line));
    std::memcpy(msg.body(), line, msg.body_length());
    msg.encode_header();
    // c.write(msg);

    for(int i = 0; i < 1000; ++i) {
    

      // boost::asio::steady_timer timer(io_context, boost::asio::chrono::seconds(5));
      // timer.async_wait([&c, &msg](boost::system::error_code /*ec*/){
      //   c.write(msg);
      // });

      c.write(msg);
      sleep(1);
    }

    sleep(5);

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}