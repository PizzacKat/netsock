#ifndef NETSOCK_IP_ADDRESS_HPP
#define NETSOCK_IP_ADDRESS_HPP

#if defined(__WIN32__) || defined(__WIN64__)
#include <winsock2.h>
#include <ws2ipdef.h>
#endif
#include <string>
#include <vector>
#include "types.hpp"
#include <stdexcept>

namespace netsock {
    class address_exception : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class ip_address {
    public:
        ip_address();
        explicit ip_address(const std::string &address);
        explicit ip_address(const std::vector<unsigned char> &bytes);
        ip_address(const std::vector<unsigned char> &bytes, unsigned long scopeId);
        ip_address(const std::vector<unsigned short> &values, unsigned long scopeId);
        explicit ip_address(unsigned int address);

        static unsigned short host_to_network(unsigned short value);
        static unsigned int host_to_network(unsigned int value);
        static unsigned long host_to_network(unsigned long value);
        static unsigned long long host_to_network(unsigned long long value);

        static unsigned short network_to_host(unsigned short value);
        static unsigned int network_to_host(unsigned int value);
        static unsigned long network_to_host(unsigned long value);
        static unsigned long long network_to_host(unsigned long long value);

        static ip_address parse(const char *bytes, int size);
        static ip_address parse(const std::string &address);

        void to_bytes(char *bytes, int &size) const;

        void set_scope_id(unsigned int scopeId);

        [[nodiscard]] ip_address map_to_ipv6() const;
        [[nodiscard]] ip_address map_to_ipv4() const;

        [[nodiscard]] unsigned int address() const;
        [[nodiscard]] address_family family() const;
        [[nodiscard]] unsigned int scopeId() const;
        [[nodiscard]] bool ipv4_mapped_ipv6() const;

        bool operator==(const ip_address &address) const;

        static ip_address any;
        static ip_address loopback;
        static ip_address broadcast;

        static ip_address any6;
        static ip_address loopback6;
    private:
        inline void writeBytesIPv6(char *out) const;

        inline void writeBytesIPv4(char *out) const;

        unsigned int m_address{0};
        unsigned short m_values[8]{0, 0, 0, 0, 0, 0, 0, 0};
        address_family m_family{inet};
    };
}

#endif //NETSOCK_IP_ADDRESS_HPP
