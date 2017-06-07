//
// Created by zdh on 17/5/28.
//
#include <Boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;
using namespace std;

class asio_server {
    typedef asio_server this_type;
    typedef ip::tcp::acceptor acceptor_type;
    typedef ip::tcp::endpoint endpoint_type;
    typedef ip::tcp::socket socket_type;
    typedef ip::address address_type;
    typedef boost::shared_ptr<socket_type> sock_ptr;

    io_service m_service;
    sock_ptr sock;
    endpoint_type m_endpoint;
    acceptor_type acc;
public:
    asio_server():sock(new ip::tcp::socket(m_service)), m_endpoint(address_type::from_string("127.0.0.1"), 9001),acc(m_service,m_endpoint) {

    }

    void start() {
        start_accept(sock);
        m_service.run();
    }

    void start_accept(sock_ptr sock) {
        acc.async_accept(*sock, boost::bind(&this_type::handle_accept,this, sock, _1) );
    }

    void handle_accept(sock_ptr sock, const boost::system::error_code &err) {
//        if (err) return;
        // 从这里开始, 你可以从socket读取或者写入
        char data[512];
        size_t len = sock->read_some(buffer(data, sizeof(data)));
        if ( len > 0)
            std::cout << data << std::endl;
            write(*sock, buffer("ok", 2));

        acc.async_accept(*sock, boost::bind(&this_type::handle_accept,this, sock, _1) );
    }
};

int adfasdf() {
    asio_server server;
    server.start();
    std::cout << "Hello, asio server!" << std::endl;

    return 0;
}

