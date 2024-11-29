#ifndef UDP_SOCKET_HPP
#define UDP_SOCKET_HPP

#include "ip_address.hpp"
#include "ip_endpoint.hpp"
#include "socket.hpp"

namespace netsock {
    class udp_socket {
    public:
        explicit udp_socket(address_family family = af_inet);
        udp_socket(netsock::socket &&socket);

        void bind(const ip_endpoint &endpoint);
        void bind(const ip_address &address, uint16_t port);

        std::size_t sendto(const std::byte *data, std::size_t len, const ip_endpoint &to);
        std::size_t recvfrom(std::byte *data, std::size_t len, ip_endpoint &from);
        std::size_t sendto(std::span<const std::byte> span, const ip_endpoint &to);
        std::size_t recvfrom(std::span<std::byte> span, ip_endpoint &from);

        [[nodiscard]] ip_endpoint address() const;

        [[nodiscard]] int poll(int events, std::chrono::milliseconds timeout = poll_wait) const;
        [[nodiscard]] bool can_read() const;
        [[nodiscard]] bool can_send() const;
        void wait_read() const;
        void wait_send() const;

        [[nodiscard]] netsock::socket &socket();
        [[nodiscard]] const netsock::socket &socket() const;
    private:
        netsock::socket _socket;
    };

    class udp_connection {
    public:
        explicit udp_connection(address_family family = af_inet);
        udp_connection(netsock::socket &&socket);
        udp_connection(udp_socket &&socket);
        udp_connection(udp_socket &&local, const ip_endpoint &remote);
        explicit udp_connection(const ip_endpoint &endpoint);
        udp_connection(const ip_endpoint &local, const ip_endpoint &remote);
        udp_connection(const ip_address &address, uint16_t port);

        void bind(const ip_endpoint &endpoint);
        void bind(const ip_address &address, uint16_t port);

        void connect(const ip_endpoint &endpoint);
        void connect(const ip_address &address, uint16_t port);

        std::size_t send(const std::byte *data, std::size_t len);
        std::size_t recv(std::byte *data, std::size_t len);
        std::size_t send(std::span<const std::byte> span);
        std::size_t recv(std::span<std::byte> span);

        [[nodiscard]] ip_endpoint address() const;

        [[nodiscard]] int poll(int events, std::chrono::milliseconds timeout = poll_wait) const;
        [[nodiscard]] bool can_read() const;
        [[nodiscard]] bool can_send() const;
        void wait_read() const;
        void wait_send() const;

        [[nodiscard]] netsock::socket &socket();
        [[nodiscard]] const netsock::socket &socket() const;
    private:
        netsock::socket _socket;
    };
}

#endif //UDP_SOCKET_HPP
