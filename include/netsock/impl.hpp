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
    class impl {
    public:
        impl()= default;

        virtual socket_t create_socket(address_family family, socket_type type, ip_protocol protocol)= 0;
        virtual void close_socket(socket_t socket)= 0;
        virtual void bind(socket_t socket, const socket_address &address)= 0;
        virtual void listen(socket_t socket, int backlog)= 0;
        virtual socket_t accept(socket_t socket, socket_address *out)= 0;
        virtual void connect(socket_t socket, const socket_address &address)= 0;
        virtual std::size_t send(socket_t socket, const std::byte *data, std::size_t len)= 0;
        virtual std::size_t recv(socket_t socket, std::byte *data, std::size_t len)= 0;
        virtual std::size_t sendto(socket_t socket, const std::byte *data, std::size_t len, const socket_address &to)= 0;
        virtual std::size_t recvfrom(socket_t socket, std::byte *data, std::size_t len, socket_address &from)= 0;

        virtual void set_option(socket_t socket, option::socket option, const void *value, std::size_t len)= 0;
        virtual void get_option(socket_t socket, option::socket option, void *value, std::size_t &len)= 0;
        virtual void set_option(socket_t socket, option::tcp option, const void *value, std::size_t len)= 0;
        virtual void get_option(socket_t socket, option::tcp option, void *value, std::size_t &len)= 0;

        virtual void parse_address(const std::string &address, addrv4_t &out)= 0;
        virtual void parse_address(const std::string &address, addrv6_t &out)= 0;

        virtual socket_address get_socket_address(socket_t socket)= 0;

        virtual socket_address convert_address(const ip_endpoint &address)= 0;
        virtual ip_endpoint convert_address(const socket_address &address)= 0;

        socket_t accept(const socket_t socket, socket_address &out) {
            return accept(socket, &out);
        }

        socket_t accept(const socket_t socket) {
            return accept(socket, nullptr);
        }

        std::size_t send(const socket_t socket, const std::span<const std::byte> data) {
            return send(socket, data.data(), data.size());
        }

        std::size_t recv(const socket_t socket, const std::span<std::byte> data) {
            return recv(socket, data.data(), data.size());
        }

        std::size_t sendto(const socket_t socket, const std::span<const std::byte> data, const socket_address &to) {
            return sendto(socket, data.data(), data.size(), to);
        }

        std::size_t recvfrom(const socket_t socket, const std::span<std::byte> data, socket_address &from) {
            return recvfrom(socket, data.data(), data.size(), from);
        }

        virtual std::size_t poll(poll_list &list, std::chrono::milliseconds timeout)= 0;
        virtual int poll(socket_t socket, int events, std::chrono::milliseconds timeout)= 0;

        virtual int error_code()= 0;

        virtual ~impl()= default;
    };

    inline std::shared_ptr<impl> impl;

    template <typename T>
    class impl_initializer {
    public:
        template <typename... Args>
        explicit impl_initializer(Args &&...args) {
            impl = std::dynamic_pointer_cast<class impl>(std::make_shared<T>(std::forward<Args>(args)...));
        }
    };
}

#endif //IMPL_HPP
