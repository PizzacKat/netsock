#include "netsock/tcp_listener.hpp"

#include "netsock/impl.hpp"

namespace netsock {
    tcp_listener::tcp_listener(const address_family family): _socket(family, sock_stream, ipproto_tcp) {

    }

    tcp_listener::tcp_listener(netsock::socket &&socket): _socket(std::move(socket)){

    }

    void tcp_listener::bind(const uint16_t port) {
        _socket.bind(ip_endpoint({}, port));
    }

    void tcp_listener::bind(const ip_endpoint &endpoint) {
        _socket.bind(endpoint);
    }

    void tcp_listener::bind(const ip_address &address, const uint16_t port) {
        _socket.bind(ip_endpoint(address, port));
    }

    void tcp_listener::bind(const std::string &ip) {
        _socket.bind(ip_endpoint(ip));
    }

    void tcp_listener::bind(const std::string &address, const uint16_t port) {
        _socket.bind(ip_endpoint(address, port));
    }

    void tcp_listener::listen(const int backlog) {
        _socket.listen(backlog);
        _listening = true;
    }

    socket tcp_listener::accept() {
        class socket tmp = _socket.accept();
        const int nodelay = _nodelay;
        impl::set_option(tmp.handle(), option::tcp_nodelay, &nodelay, sizeof(int));
        return tmp;
    }

    tcp_client tcp_listener::accept_client() {
        tcp_client tmp = _socket.accept();
        tmp.no_delay(_nodelay);
        return tmp;
    }

    void tcp_listener::no_delay(const bool enabled) {
        _nodelay = enabled;
    }

    bool tcp_listener::no_delay() const {
        return _nodelay;
    }

    ip_endpoint tcp_listener::address() const {
        return impl::convert_address(_socket.address());
    }

    bool tcp_listener::listening() const {
        return _listening;
    }

    void tcp_listener::close() {
        _listening = false;
        _socket.close();
    }

    socket &tcp_listener::socket() {
        return _socket;
    }

    const socket &tcp_listener::socket() const {
        return _socket;
    }

    tcp_listener::~tcp_listener() {
        _listening = false;
    }

}
