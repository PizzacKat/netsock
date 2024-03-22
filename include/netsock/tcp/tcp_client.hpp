#ifndef NETSOCK_TCP_CLIENT_HPP
#define NETSOCK_TCP_CLIENT_HPP

#include "netsock/socket/socket.hpp"
#include "netsock/network_stream.hpp"
#include <vector>

namespace netsock {
    class tcp_client {
    public:
        tcp_client();
        explicit tcp_client(address_family family);
        tcp_client(const std::string &address, unsigned short port);
        explicit tcp_client(socket &&client);

        static tcp_client create();

        void connect(const ip_address &address, unsigned short port);
        void connect(const std::vector<ip_address> &addresses, unsigned short port);
        void connect(const std::string &address, unsigned short port);
        [[nodiscard]] bool connected() const;
        [[nodiscard]] bool pending();

        [[nodiscard]] const socket &client() const;
        socket &client();

        network_stream &stream();

        void close();
    private:
        socket m_client;
        network_stream m_stream;
        friend class tcp_listener;
    };
}

#endif //NETSOCK_TCP_CLIENT_HPP
