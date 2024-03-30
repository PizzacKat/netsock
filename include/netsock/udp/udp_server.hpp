#ifndef NETSOCK_UDP_SERVER_HPP
#define NETSOCK_UDP_SERVER_HPP

#include <netsock/socket/socket.hpp>

namespace netsock {
    class udp_server {
    public:
        udp_server(const std::string &address, unsigned short port);
        explicit udp_server(const ip_endpoint &endpoint);
        udp_server(address_family family, const ip_endpoint &endpoint);
        udp_server(udp_server &&server) noexcept;
        udp_server &operator=(udp_server &&server) noexcept;

        [[nodiscard]] const socket &server() const;
        socket &server();

        retsize_t write_to(const ip_endpoint &endpoint, const char *data, datsize_t length, size_t offset = 0);
        size_t write_to(const ip_endpoint &endpoint, const std::vector<char> &data);
        retsize_t read_from(ip_endpoint &endpoint, char *out, datsize_t amount, size_t offset = 0);
        std::vector<char> read_from(ip_endpoint &endpoint, size_t amount);

        void close();

        ~udp_server();
    private:
        socket m_server;
    };
};

#endif //NETSOCK_UDP_SERVER_HPP
