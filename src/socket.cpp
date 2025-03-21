#include "netsock/socket.hpp"

#include "netsock/impl.hpp"

namespace netsock {
    constexpr socket_t _invalid_socket{~(socket_t)0};

    socket::invalid_socket_t::operator socket() const {
        return _invalid_socket;
    }

    const socket::invalid_socket_t socket::invalid_socket{};

    socket::socket(): _socket(_invalid_socket) {

    }

    socket::socket(const socket_t socket, const bool connected): _connected(connected), _socket(socket) {

    }

    socket::socket(const address_family family, const socket_type type, const ip_protocol protocol): socket(impl::create_socket(family, type, protocol)) {
        if (_socket == _invalid_socket)
            throw socket_error("error creating socket");
    }

    socket::socket(socket &&other) noexcept: _connected(other._connected), _socket(other._socket) {
        other._socket = _invalid_socket;
    }

    socket &socket::operator=(socket &&other) noexcept {
        close();
        _socket = other._socket;
        _connected = other._connected;
        other._socket = _invalid_socket;
        other._connected = false;
        return *this;
    }

    socket::operator bool() const {
        return _socket != _invalid_socket;
    }

    bool socket::connected() const {
        return _connected;
    }

    socket_t socket::handle() const {
        return _socket;
    }

    void socket::connect(const socket_address &address) {
        impl::connect(_socket, address);
        _connected = true;
    }

    void socket::bind(const socket_address &address) {
        impl::bind(_socket, address);
    }

    void socket::listen(const int backlog) {
        impl::listen(_socket, backlog);
    }

    socket socket::accept(socket_address &address) {
        return {impl::accept(_socket, address), true};
    }

    socket socket::accept() {
        return {impl::accept(_socket), true};
    }

    std::size_t socket::recv(std::byte* data, const std::size_t len) {
        if (!_connected)
            throw socket_error("socket not connected");
        const std::size_t res = impl::recv(_socket, data, len);
        if (res == 0)
            close();
        return res;
    }

    std::size_t socket::send(const std::byte* data, const std::size_t len) {
        if (!_connected)
            throw socket_error("socket not connected");
        return impl::send(_socket, data, len);
    }

    std::size_t socket::recv(const std::span<std::byte> span) {
        if (!_connected)
            throw socket_error("socket not connected");
        const std::size_t res = impl::recv(_socket, span.data(), span.size());
        if (res == 0)
            close();
        return res;
    }

    std::size_t socket::send(const std::span<const std::byte> span) {
        if (!_connected)
            throw socket_error("socket not connected");
        return impl::send(_socket, span.data(), span.size());
    }

    std::size_t socket::recvfrom(std::byte* data, const std::size_t len, socket_address &from) {
        const std::size_t res = impl::recvfrom(_socket, data, len, from);
        if (res == 0)
            close();
        return res;
    }

    std::size_t socket::sendto(const std::byte* data, const std::size_t len, const socket_address &to) {
        return impl::sendto(_socket, data, len, to);
    }

    std::size_t socket::recvfrom(const std::span<std::byte> span, socket_address &from) {
        const std::size_t res = impl::recvfrom(_socket, span.data(), span.size(), from);
        if (res == 0)
            close();
        return res;
    }

    std::size_t socket::sendto(const std::span<const std::byte> span, const socket_address &to) {
        return impl::sendto(_socket, span.data(), span.size(), to);
    }

    int socket::poll(const int events, const std::chrono::milliseconds timeout) const {
        return impl::poll(_socket, events, timeout);
    }

    bool socket::can_read() const {
        return poll(pollin, poll_nowait) & pollin;
    }

    bool socket::can_send() const {
        return poll(pollout, poll_nowait) & pollout;
    }

    bool socket::hanged_up() const {
        return poll(pollout, poll_nowait) & pollhang;
    }

    void socket::wait_read() const {
        while (poll(pollin) == 0);
    }

    void socket::wait_send() const {
        while (poll(pollout) == 0);
    }

    socket_address socket::address() const {
        return impl::get_socket_address(_socket);
    }

    void socket::send_buffer(const std::size_t bytes) {
        impl::set_option(_socket, option::so_sndbuf, &bytes, sizeof(bytes));
    }

    std::size_t socket::send_buffer() const {
        std::size_t bytes = 0;
        std::size_t len = sizeof(bytes);
        impl::get_option(_socket, option::so_sndbuf, &bytes, len);
        return bytes;
    }

    void socket::recv_buffer(const std::size_t bytes) {
        impl::set_option(_socket, option::so_rcvbuf, &bytes, sizeof(bytes));
    }

    std::size_t socket::recv_buffer() const {
        std::size_t bytes = 0;
        std::size_t len = sizeof(bytes);
        impl::get_option(_socket, option::so_rcvbuf, &bytes, len);
        return bytes;
    }

    void socket::send_timeout(const std::chrono::milliseconds timeout) {
        impl::set_option(_socket, option::so_sndtimeo, &timeout, sizeof(timeout));
    }

    std::chrono::milliseconds socket::send_timeout() const {
        int64_t count = 0;
        std::size_t len = sizeof(count);
        impl::get_option(_socket, option::so_sndtimeo, &count, len);
        return std::chrono::milliseconds(count);
    }

    void socket::recv_timeout(const std::chrono::milliseconds timeout) {
        impl::set_option(_socket, option::so_rcvtimeo, &timeout, sizeof(timeout));
    }

    std::chrono::milliseconds socket::recv_timeout() const {
        int64_t count = 0;
        std::size_t len = sizeof(count);
        impl::get_option(_socket, option::so_rcvtimeo, &count, len);
        return std::chrono::milliseconds(count);
    }

    void socket::reuse_address(const bool reuse) {
        const int tmp = reuse;
        impl::set_option(_socket, option::so_reuseaddr, &tmp, sizeof(tmp));
    }

    bool socket::reuse_address() const {
        int res = false;
        std::size_t len = sizeof(res);
        impl::get_option(_socket, option::so_reuseaddr, &res, len);
        return res;
    }

    void socket::reuse_port(const bool reuse) {
        const int tmp = reuse;
        impl::set_option(_socket, option::so_reuseport, &tmp, sizeof(tmp));
    }

    bool socket::reuse_port() const {
        int res = false;
        std::size_t len = sizeof(res);
        impl::get_option(_socket, option::so_reuseport, &res, len);
        return res;
    }

    bool socket::operator==(const socket &other) const {
        return _socket == other._socket;
    }

    bool socket::operator!=(const socket &other) const {
        return _socket != other._socket;
    }

    void socket::close() {
        _connected = false;
        if (_socket != _invalid_socket && !_closed)
            impl::close_socket(_socket);
        _closed = true;
    }

    socket::~socket() {
        close();
    }
}
