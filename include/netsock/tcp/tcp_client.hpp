#ifndef NETSOCK_TCP_CLIENT_HPP
#define NETSOCK_TCP_CLIENT_HPP

#include "netsock/socket/socket.hpp"
#include <vector>

namespace netsock {
    class tcp_client {
    public:
        tcp_client();
        explicit tcp_client(address_family family);
        tcp_client(const std::string &address, unsigned short port);
        explicit tcp_client(socket &&client);
        tcp_client(tcp_client &&) noexcept;
        tcp_client &operator=(tcp_client &&) noexcept;

        static tcp_client create();

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

        void close();

        ~tcp_client();
    private:
        socket m_client;
        friend class tcp_listener;
    };
}

#endif //NETSOCK_TCP_CLIENT_HPP
