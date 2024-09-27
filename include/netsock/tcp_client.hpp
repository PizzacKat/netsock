#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <memory>

#include "socket.hpp"
#include "ip_address.hpp"
#include "ip_endpoint.hpp"
#include "nstream.hpp"

namespace netsock {
    class tcp_client {
    public:
        explicit tcp_client(address_family family = af_inet);
        tcp_client(netsock::socket &&socket);
        explicit tcp_client(const ip_endpoint &endpoint);
        tcp_client(const ip_address &address, uint16_t port);

        void connect(const ip_endpoint &endpoint);
        void connect(const ip_address &address, uint16_t port);

        void no_delay(bool enabled) const;
        [[nodiscard]] bool no_delay() const;

        [[nodiscard]] ip_endpoint address() const;

        [[nodiscard]] bool connected() const;

        [[nodiscard]] int poll(int events, std::chrono::milliseconds timeout = poll_wait);
        [[nodiscard]] bool can_read();
        [[nodiscard]] bool can_send();
        void wait_read();
        void wait_send();

        void close();

        [[nodiscard]] nstream &stream();
        [[nodiscard]] const netsock::socket &socket() const;
        [[nodiscard]] netsock::socket &socket();
    private:
        netsock::socket _socket;
        std::shared_ptr<nstream> _stream;
    };
}

#endif //TCP_CLIENT_HPP
