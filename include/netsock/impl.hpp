#ifndef IMPL_HPP
#define IMPL_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "types.hpp"
#include "socket_address.hpp"
#include "enum.hpp"
#include "ip_endpoint.hpp"
#include "poll_list.hpp"

namespace netsock::impl {
    socket_t create_socket(address_family family, socket_type type, ip_protocol protocol);
    void close_socket(socket_t socket);
    void bind(socket_t socket, const socket_address &address);
    void listen(socket_t socket, int backlog);
    socket_t accept(socket_t socket, socket_address *out);
    void connect(socket_t socket, const socket_address &address);
    std::size_t send(socket_t socket, const std::byte *data, std::size_t len);
    std::size_t recv(socket_t socket, std::byte *data, std::size_t len);
    std::size_t sendto(socket_t socket, const std::byte *data, std::size_t len, const socket_address &to);
    std::size_t recvfrom(socket_t socket, std::byte *data, std::size_t len, socket_address &from);

    void set_option(socket_t socket, option::socket option, const void *value, std::size_t len);
    void get_option(socket_t socket, option::socket option, void *value, std::size_t &len);
    void set_option(socket_t socket, option::tcp option, const void *value, std::size_t len);
    void get_option(socket_t socket, option::tcp option, void *value, std::size_t &len);

    void parse_address(const std::string &address, addrv4_t &out);
    void parse_address(const std::string &address, addrv6_t &out);

    socket_address get_socket_address(socket_t socket);

    std::vector<socket_address> resolve_hosts(const std::string &hostname);

    socket_address convert_address(const ip_endpoint &address);
    ip_endpoint convert_address(const socket_address &address);

    inline socket_t accept(const socket_t socket, socket_address &out) {
        return accept(socket, &out);
    }

    inline socket_t accept(const socket_t socket) {
        return accept(socket, nullptr);
    }

    inline std::size_t send(const socket_t socket, const std::span<const std::byte> data) {
        return send(socket, data.data(), data.size());
    }

    inline std::size_t recv(const socket_t socket, const std::span<std::byte> data) {
        return recv(socket, data.data(), data.size());
    }

    inline std::size_t sendto(const socket_t socket, const std::span<const std::byte> data, const socket_address &to) {
        return sendto(socket, data.data(), data.size(), to);
    }

    inline std::size_t recvfrom(const socket_t socket, const std::span<std::byte> data, socket_address &from) {
        return recvfrom(socket, data.data(), data.size(), from);
    }

    std::size_t poll(poll_list &list, std::chrono::milliseconds timeout);
    int poll(socket_t socket, int events, std::chrono::milliseconds timeout);

    int error_code();
}

#endif //IMPL_HPP
