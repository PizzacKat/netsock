#ifndef NETSOCK_IP_ENDPOINT_HPP
#define NETSOCK_IP_ENDPOINT_HPP

#include "ip_address.hpp"
#include "socket_address.hpp"

namespace netsock {
    class ip_endpoint {
    public:
        ip_endpoint();
        ip_endpoint(const ip_address &address, unsigned short port);
        explicit ip_endpoint(const socket_address &address);

        [[nodiscard]] socket_address serialize() const;
        [[nodiscard]] address_family family() const;

        static ip_endpoint parse(const std::string &string);

        bool operator==(const ip_endpoint &endpoint) const;

        ip_address address;
        unsigned short port;
    };
}

#endif //NETSOCK_IP_ENDPOINT_HPP
