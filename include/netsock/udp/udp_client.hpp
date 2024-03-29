#ifndef NETSOCK_UDP_CLIENT_HPP
#define NETSOCK_UDP_CLIENT_HPP

#include "netsock/socket/socket.hpp"
#include "netsock/ip_endpoint.hpp"
#include <vector>

namespace netsock {
    class udp_client {
    public:
        udp_client();
        explicit udp_client(address_family family);
        udp_client(unsigned short port);
        explicit udp_client(socket &&client);

        static udp_client create();

        void connect(const ip_address &address, unsigned short port);
        void connect(const std::vector<ip_address> &addresses, unsigned short port);
        void connect(const std::string &address, unsigned short port);
        [[nodiscard]] bool connected() const;
        [[nodiscard]] bool pending();

        [[nodiscard]] const socket &client() const;
        socket &client();

        retsize_t write(const char *data, datsize_t length, size_t offset = 0);
        size_t write(const std::vector<char> &data);
        retsize_t read(char *out, datsize_t amount, size_t offset = 0);
        std::vector<char> read(size_t amount);

        retsize_t write_to(const ip_endpoint &endpoint, const char *data, datsize_t length, size_t offset = 0);
        size_t write_to(const ip_endpoint &endpoint, const std::vector<char> &data);
        retsize_t write_to(const char *data, datsize_t length, size_t offset = 0);
        size_t write_to(const std::vector<char> &data);
        retsize_t read_from(ip_endpoint &endpoint, char *out, datsize_t amount, size_t offset = 0);
        std::vector<char> read_from(ip_endpoint &endpoint, size_t amount);
        retsize_t read_from(char *out, datsize_t amount, size_t offset = 0);
        std::vector<char> read_from(size_t amount);

        void close();

        ~udp_client();
    private:
        socket m_client;
    };
}

#endif //NETSOCK_UDP_CLIENT_HPP
