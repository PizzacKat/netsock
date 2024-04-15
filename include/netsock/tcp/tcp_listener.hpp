#ifndef NETSOCK_TCP_LISTENER_HPP
#define NETSOCK_TCP_LISTENER_HPP

#include "tcp_client.hpp"
#include "../ip_endpoint.hpp"
#include <memory>

namespace netsock {
    class tcp_listener {
    public:
        tcp_listener(const ip_address &address, unsigned short port);
        explicit tcp_listener(const ip_endpoint &endpoint);

        void start();
        void start(int connections);

        void stop();

        bool pending();

        socket accept_socket();
        tcp_client accept_client();

        [[nodiscard]] const socket &server() const;
        socket &server();

        [[nodiscard]] bool active() const;

    private:
        socket m_server;
        bool m_active = false;
    };
}

#endif //NETSOCK_TCP_LISTENER_HPP
