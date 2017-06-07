//
// Created by zdh on 17/6/7.
//

#include <random>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "protlcal.h"

class session {
public:
    session(boost::asio::io_service &io_service) : socket_(io_service) {}

    boost::asio::ip::tcp::socket &socket() {
        return socket_;
    }

    void start() {
        async_read_buf(buf_, boost::bind(&session::handle_read_message, this, _1));
    }

private:
    void handle_read_message(boost::system::error_code ec) {
        if (ec) {
            delete this;
            return;
        }

        boost::archive::binary_iarchive ia(buf_);
        message_t msg;
        ia >> msg;  //得到message结构体

        std::cout << msg.str << std::endl;

        buf_.consume(buf_.size());//清空buf
        boost::archive::binary_oarchive oa(buf_);

        std::uniform_int_distribution<unsigned> u(1, 10); // 分部类型(生成1-10之间均匀分布的随机数)
        std::default_random_engine e; // 生成无符号随机整数
        int num = u(e); //生成返回的message结构体的数量
        oa << num;  //加上数量
        for (int i = 0; i < num; ++i) {
            oa << msg;
        }
        async_write_buf(buf_, [this](boost::system::error_code) { delete this; });
    }

private:
    //回调函数
    typedef std::function<void(boost::system::error_code ec)> callback_t;

    //读取buf
    void async_read_buf(boost::asio::streambuf &buf, callback_t callback) {
        boost::asio::async_read(socket_, boost::asio::buffer(&size_, sizeof(int)),
                                boost::bind(&session::handle_read_size, this, boost::ref(buf), callback,
                                            boost::asio::placeholders::error));
    }

    void handle_read_size(boost::asio::streambuf &buf, callback_t callback, boost::system::error_code ec) {
        if (ec) {
            delete this;
            return;
        }

        boost::asio::async_read(socket_, buf, boost::asio::transfer_exactly(size_),
                                boost::bind(callback, boost::asio::placeholders::error));
    }

    //写入buf
    void async_write_buf(boost::asio::streambuf &buf, callback_t callback) {
        size_ = buf.size();
        boost::asio::async_write(socket_, boost::asio::buffer(&size_, sizeof(int)),
                                 boost::bind(&session::handle_write_size, this, boost::ref(buf), callback,
                                             boost::asio::placeholders::error));
    }

    void handle_write_size(boost::asio::streambuf &buf, callback_t callback, boost::system::error_code ec) {
        if (ec) {
            delete this;
            return;
        }

        boost::asio::async_write(socket_, buf, boost::asio::transfer_exactly(size_),
                                 boost::bind(callback, boost::asio::placeholders::error));
    }

    int size_;
    boost::asio::streambuf buf_;
    boost::asio::ip::tcp::socket socket_;
};

class server {
public:
    server(boost::asio::io_service &io_service, short port = 9001)
            : io_service_(io_service),
              acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port)) {
        start_accept();
    }

private:
    void start_accept() {
        session *new_session = new session(io_service_);
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(session *new_session,
                       const boost::system::error_code &error) {
        if (!error) {
            new_session->start();
        } else {
            delete new_session;
        }

        start_accept();
    }

    boost::asio::io_service &io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

int main(int argc, char *argv[]) {
    try {
        boost::asio::io_service io_service;

        server s(io_service);

        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}