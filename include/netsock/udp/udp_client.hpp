#ifndef NETSOCK_UDP_CLIENT_HPP
#define NETSOCK_UDP_CLIENT_HPP

#include "netsock/socket/socket.hpp"
#include <vector>

namespace netsock {
    class upd_client {
    public:
        upd_client();
        explicit upd_client(address_family family);
        upd_client(const std::string &address, unsigned short port);
        explicit upd_client(socket &&client);
        upd_client(upd_client &&) noexcept;
        upd_client &operator=(upd_client &&) noexcept;

        static upd_client create();

        void connect(const ip_address &address, unsigned short port);
        void connect(const std::vector<ip_address> &addresses, unsigned short port);
        void connect(const std::string &address, unsigned short port);
        [[nodiscard]] bool connected() const;
        [[nodiscard]] bool pending();

        [[nodiscard]] const socket &client() const;
        socket &client();

        void close();

        ~tcp_client();
    private:
        socket m_client;
        friend class udp_listener;
    };
}

#endif //NETSOCK_UDP_CLIENT_HPP
