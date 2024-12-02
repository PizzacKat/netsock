#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <memory>

#include "socket.hpp"
#include "ip_address.hpp"
#include "ip_endpoint.hpp"

namespace netsock {
    class tcp_client {
    public:
        explicit tcp_client(address_family family = af_inet);
        tcp_client(netsock::socket &&socket, const ip_endpoint &remote = {});
        explicit tcp_client(const ip_endpoint &endpoint);
        tcp_client(const ip_address &address, uint16_t port);

        void connect(const ip_endpoint &endpoint);
        void connect(const ip_address &address, uint16_t port);

        std::size_t send(const std::byte *data, std::size_t len);
        std::size_t recv(std::byte *data, std::size_t len);
        std::size_t send(std::span<const std::byte> span);
        std::size_t recv(std::span<std::byte> span);

        void no_delay(bool enabled) const;
        [[nodiscard]] bool no_delay() const;

        [[nodiscard]] ip_endpoint address() const;
        [[nodiscard]] ip_endpoint remote_address() const;

        [[nodiscard]] bool connected() const;

        [[nodiscard]] int poll(int events, std::chrono::milliseconds timeout = poll_wait) const;
        [[nodiscard]] bool can_read() const;
        [[nodiscard]] bool can_send() const;
        void wait_read() const;
        void wait_send() const;

        void close();

        [[nodiscard]] const netsock::socket &socket() const;
        [[nodiscard]] netsock::socket &socket();
    private:
        netsock::socket _socket;
        ip_endpoint _remote;
    };
}

#endif //TCP_CLIENT_HPP
