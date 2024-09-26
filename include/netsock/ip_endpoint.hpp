#ifndef IP_ENDPOINT_HPP
#define IP_ENDPOINT_HPP

#include "socket_address.hpp"
#include "ip_address.hpp"

namespace netsock {
    class ip_endpoint {
    public:
        ip_endpoint();
        ip_endpoint(const ip_address &address, uint16_t port = 0);
        ip_endpoint(const std::string &ip);
        ip_endpoint(const char *ip);

        [[nodiscard]] const ip_address &address() const;
        [[nodiscard]] uint16_t port() const;

        operator socket_address() const;
    private:
        ip_address _address;
        uint16_t _port;
    };

    inline ip_endpoint operator&(const ip_address &address, const uint16_t port) {
        return {address, port};
    }
}

#endif //IP_ENDPOINT_HPP
