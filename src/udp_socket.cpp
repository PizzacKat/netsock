#include "netsock/udp_socket.hpp"

#include <netsock/impl.hpp>

namespace netsock {
    udp_socket::udp_socket(const address_family family): _socket(family, sock_dgram, ipproto_udp) {

    }

    udp_socket::udp_socket(netsock::socket&& socket): _socket(std::move(socket)) {

    }

    void udp_socket::bind(const ip_endpoint& endpoint) {
        _socket.bind(endpoint);
    }

    void udp_socket::bind(const ip_address& address, const uint16_t port) {
        _socket.bind(ip_endpoint(address, port));
    }

    std::size_t udp_socket::sendto(const std::byte* data, const std::size_t len, const ip_endpoint& to) {
        return _socket.sendto(data, len, to);
    }

    std::size_t udp_socket::recvfrom(std::byte* data, const std::size_t len, ip_endpoint& from) {
        socket_address tmp;
        const std::size_t recv = _socket.recvfrom(data, len, tmp);
        from = impl::convert_address(tmp);
        from = impl::convert_address(tmp);
        return recv;
    }

    std::size_t udp_socket::recvfrom(const std::span<std::byte> span, ip_endpoint& from) {
        socket_address tmp;
        const std::size_t recv = _socket.recvfrom(span, tmp);
        from = impl::convert_address(tmp);
        return recv;
    }

    std::size_t udp_socket::sendto(const std::span<const std::byte> span, const ip_endpoint& to) {
        return _socket.sendto(span, to);
    }

    ip_endpoint udp_socket::address() const {
        return impl::convert_address(_socket.address());
    }

    int udp_socket::poll(const int events, const std::chrono::milliseconds timeout) {
        return _socket.poll(events, timeout);
    }

    bool udp_socket::can_read() {
        return _socket.can_read();
    }

    bool udp_socket::can_send() {
        return _socket.can_send();
    }

    void udp_socket::wait_read() {
        _socket.wait_read();
    }

    void udp_socket::wait_send() {
        _socket.wait_send();
    }

    socket& udp_socket::socket() {
        return _socket;
    }

    const socket& udp_socket::socket() const {
        return _socket;
    }

    udp_connection::udp_connection(const address_family family): _socket(family, sock_dgram, ipproto_udp) {

    }

    udp_connection::udp_connection(netsock::socket&& socket): _socket(std::move(socket)) {

    }

    udp_connection::udp_connection(udp_socket&& socket): _socket(std::move(socket.socket())) {

    }

    udp_connection::udp_connection(udp_socket&& local, const ip_endpoint& remote): _socket(std::move(local.socket())) {
        _socket.connect(remote);
    }

    udp_connection::udp_connection(const ip_endpoint& endpoint): _socket(endpoint.address().family(), sock_dgram, ipproto_udp) {
        _socket.bind(endpoint);
    }

    udp_connection::udp_connection(const ip_endpoint& local, const ip_endpoint& remote): _socket(local.address().family(), sock_dgram, ipproto_udp) {
        if (local.address().family() != remote.address().family())
            throw std::invalid_argument("address families must be the same");
        _socket.bind(local);
        _socket.connect(remote);
    }

    udp_connection::udp_connection(const ip_address& address, const uint16_t port): udp_connection(ip_endpoint(address, port)) {

    }

    void udp_connection::bind(const ip_endpoint& endpoint) {
        _socket.bind(endpoint);
    }

    void udp_connection::bind(const ip_address& address, const uint16_t port) {
        _socket.bind(ip_endpoint(address, port));
    }

    void udp_connection::connect(const ip_endpoint& endpoint) {
        _socket.connect(endpoint);
    }

    void udp_connection::connect(const ip_address& address, const uint16_t port) {
        _socket.connect(ip_endpoint(address, port));
    }

    std::size_t udp_connection::send(const std::byte* data, const std::size_t len) {
        return _socket.send(data, len);
    }

    std::size_t udp_connection::recv(std::byte* data, const std::size_t len) {
        return _socket.recv(data, len);
    }

    std::size_t udp_connection::send(const std::span<const std::byte> span) {
        return _socket.send(span);
    }

    std::size_t udp_connection::recv(const std::span<std::byte> span) {
        return _socket.recv(span);
    }

    ip_endpoint udp_connection::address() const {
        return impl::convert_address(_socket.address());
    }

    int udp_connection::poll(const int events, const std::chrono::milliseconds timeout) {
        return _socket.poll(events, timeout);
    }

    bool udp_connection::can_read() {
        return _socket.can_read();
    }

    bool udp_connection::can_send() {
        return _socket.can_send();
    }

    void udp_connection::wait_read() {
        _socket.wait_read();
    }

    void udp_connection::wait_send() {
        _socket.wait_send();
    }

    socket& udp_connection::socket() {
        return _socket;
    }

    const socket& udp_connection::socket() const {
        return _socket;
    }
}
