#ifndef SOCKET_H
#define SOCKET_H

#include <atomic>
#include <cstdint>

#include "enum.hpp"
#include "types.hpp"
#include "socket_address.hpp"
#include <vector>
#include <span>

namespace netsock {
    class socket {
    public:
        struct invalid_socket_t {
            operator socket() const;
        };

        static const invalid_socket_t invalid_socket;

        socket();
        socket(socket_t socket, bool connected = false);
        socket(address_family family, socket_type type, ip_protocol protocol);
        socket(const socket& other)= delete;
        socket &operator=(const socket& other)= delete;
        socket(socket &&other) noexcept;
        socket &operator=(socket &&other) noexcept;

        explicit operator bool() const;

        [[nodiscard]] bool connected() const;

        [[nodiscard]] socket_t handle() const;
        void connect(const socket_address &address);
        void bind(const socket_address &address);
        void listen(int backlog);
        [[nodiscard]] socket accept(socket_address &address);
        [[nodiscard]] socket accept();
        std::size_t recv(std::byte *data, std::size_t len);
        std::size_t send(const std::byte *data, std::size_t len);
        std::size_t recv(std::span<std::byte> span);
        std::size_t send(std::span<const std::byte> span);
        std::size_t recvfrom(std::byte *data, std::size_t len, socket_address &from);
        std::size_t sendto(const std::byte *data, std::size_t len, const socket_address &to);
        std::size_t recvfrom(std::span<std::byte> span, socket_address& from);
        std::size_t sendto(std::span<const std::byte> span, const socket_address& to);

        [[nodiscard]] int poll(int events, std::chrono::milliseconds timeout = poll_wait) const;
        [[nodiscard]] bool can_read() const;
        [[nodiscard]] bool can_send() const;
        void wait_read() const;
        void wait_send() const;

        [[nodiscard]] socket_address address() const;

        void send_buffer(std::size_t bytes);
        [[nodiscard]] std::size_t send_buffer() const;
        void recv_buffer(std::size_t bytes);
        [[nodiscard]] std::size_t recv_buffer() const;
        void send_timeout(std::chrono::milliseconds timeout);
        [[nodiscard]] std::chrono::milliseconds send_timeout() const;
        void recv_timeout(std::chrono::milliseconds timeout);
        [[nodiscard]] std::chrono::milliseconds recv_timeout() const;

        void reuse_address(bool reuse);
        [[nodiscard]] bool reuse_address() const;
        void reuse_port(bool reuse);
        [[nodiscard]] bool reuse_port() const;

        bool operator==(const socket &other) const;
        bool operator!=(const socket &other) const;

        void close();

        ~socket();
    private:
        bool _closed = false;
        bool _connected = false;
        socket_t _socket;
    };
}

#endif //SOCKET_H
