#include "netsock/tcp_client.hpp"

#include "netsock/impl.hpp"

namespace netsock {
    tcp_client::tcp_client(const address_family family): _socket(family, sock_stream, ipproto_tcp) {

    }

    tcp_client::tcp_client(netsock::socket &&socket, const ip_endpoint &remote): _socket(std::move(socket)), _remote(remote) {

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

    std::size_t tcp_client::send(const std::byte *data, const std::size_t len) {
        return _socket.send(data, len);
    }

    std::size_t tcp_client::recv(std::byte *data, const std::size_t len) {
        return _socket.recv(data, len);
    }

    std::size_t tcp_client::send(const std::span<const std::byte> span) {
        return _socket.send(span);
    }

    std::size_t tcp_client::recv(const std::span<std::byte> span) {
        return _socket.recv(span);
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

    ip_endpoint tcp_client::remote_address() const {
        return _remote;
    }

    bool tcp_client::connected() const {
        return _socket.connected();
    }

    int tcp_client::poll(const int events, const std::chrono::milliseconds timeout) const {
        return _socket.poll(events, timeout);
    }

    bool tcp_client::can_read() const {
        return _socket.can_read();
    }

    bool tcp_client::can_send() const {
        return _socket.can_send();
    }

    void tcp_client::wait_read() const {
        _socket.wait_read();
    }

    void tcp_client::wait_send() const {
        _socket.wait_send();
    }

    void tcp_client::close() {
        _socket.close();
    }

    socket &tcp_client::socket() {
        return _socket;
    }

    const socket &tcp_client::socket() const {
        return _socket;
    }
}
