#ifndef NETSOCK_UDP_CONNECTION_HPP
#define NETSOCK_UDP_CONNECTION_HPP

#include "netsock/socket/socket.hpp"
#include "netsock/ip_endpoint.hpp"
#include <vector>

namespace netsock {
    class udp_connection {
    public:
        udp_connection();
        explicit udp_connection(address_family family);
        explicit udp_connection(unsigned short port);
        explicit udp_connection(const ip_endpoint &endpoint);
        explicit udp_connection(netsock::socket &&socket);
        udp_connection(udp_connection &&connection) noexcept;
        udp_connection &operator=(udp_connection &&connection) noexcept;

        static udp_connection create();

        void connect(const ip_address &address, unsigned short port);
        void connect(const std::vector<ip_address> &addresses, unsigned short port);
        void connect(const std::string &address, unsigned short port);
        [[nodiscard]] bool connected() const;
        [[nodiscard]] bool pending();

        [[nodiscard]] const netsock::socket &socket() const;
        netsock::socket &socket();

        retsize_t write(const char *data, datsize_t length, size_t offset = 0);
        size_t write(const std::vector<char> &data);
        retsize_t read(char *out, datsize_t amount, size_t offset = 0);
        std::vector<char> read(size_t amount);

        retsize_t write_to(const ip_endpoint &endpoint, const char *data, datsize_t length, size_t offset = 0);
        size_t write_to(const ip_endpoint &endpoint, const std::vector<char> &data);
        retsize_t read_from(ip_endpoint &endpoint, char *out, datsize_t amount, size_t offset = 0);
        std::vector<char> read_from(ip_endpoint &endpoint, size_t amount);

        void close();

        ~udp_connection();
    private:
        netsock::socket m_socket;
    };
}

#endif //NETSOCK_UDP_CONNECTION_HPP
