#include "netsock/tcp_client.hpp"

#include "netsock/impl.hpp"

namespace netsock {
    tcp_client::tcp_client(const address_family family): _socket(family, sock_stream, ipproto_tcp), _stream(std::make_shared<nstream>(_socket)) {

    }

    tcp_client::tcp_client(netsock::socket &&socket): _socket(std::move(socket)), _stream(std::make_shared<nstream>(_socket)) {

    }

    tcp_client::tcp_client(const ip_endpoint &endpoint): tcp_client(endpoint.address().family()) {
        _socket.connect(endpoint);
    }

    tcp_client::tcp_client(const ip_address &address, const uint16_t port): tcp_client(ip_endpoint(address, port)) {

    }

    void tcp_client::connect(const ip_endpoint &endpoint) {
        _socket.connect(endpoint);
    }

    void tcp_client::connect(const ip_address &address, const uint16_t port) {
        _socket.connect(ip_endpoint(address, port));
    }

    bool tcp_client::no_delay() const {
        int enabled = 0;
        std::size_t len = sizeof(enabled);
        impl::get_option(_socket.handle(), option::tcp_nodelay, &enabled, len);
        return enabled;
    }

    void tcp_client::no_delay(const bool enabled) const {
        const int tmp = enabled;
        impl::set_option(_socket.handle(), option::tcp_nodelay, &tmp, sizeof(tmp));
    }

    ip_endpoint tcp_client::address() const {
        return impl::convert_address(_socket.address());
    }

    bool tcp_client::connected() const {
        return _socket.connected();
    }

    int tcp_client::poll(const int events, const std::chrono::milliseconds timeout) {
        return _socket.poll(events, timeout);
    }

    bool tcp_client::can_read() {
        return _socket.can_read();
    }

    bool tcp_client::can_send() {
        return _socket.can_send();
    }

    void tcp_client::wait_read() {
        _socket.wait_read();
    }

    void tcp_client::wait_send() {
        _socket.wait_send();
    }

    void tcp_client::close() {
        _socket.close();
    }

    nstream &tcp_client::stream() {
        return *_stream;
    }

    socket &tcp_client::socket() {
        return _socket;
    }

    const socket &tcp_client::socket() const {
        return _socket;
    }
}
