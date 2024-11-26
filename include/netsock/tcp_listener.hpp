#ifndef TCP_LISTENER_HPP
#define TCP_LISTENER_HPP
#include <cstdint>
#include <string>

#include "enum.hpp"
#include "ip_endpoint.hpp"
#include "socket.hpp"
#include "tcp_client.hpp"

namespace netsock {
    class tcp_listener {
    public:
        explicit tcp_listener(address_family family = af_inet);
        tcp_listener(netsock::socket &&socket);

        void bind(uint16_t port);
        void bind(const ip_endpoint &endpoint);
        void bind(const ip_address &address, uint16_t port);
        void bind(const std::string &ip);
        void bind(const std::string &address, uint16_t port);

        void listen(int backlog);

        netsock::socket accept();
        tcp_client accept_client();

        void no_delay(bool enabled);
        [[nodiscard]] bool no_delay() const;

        [[nodiscard]] ip_endpoint address() const;

        [[nodiscard]] bool listening() const;
        [[nodiscard]] bool pending() const;

        void close();

        [[nodiscard]] netsock::socket &socket();
        [[nodiscard]] const netsock::socket &socket() const;

        ~tcp_listener();
    private:
        netsock::socket _socket;
        bool _nodelay = false;
        bool _listening = false;
    };
}

#endif //TCP_LISTENER_HPP
