#ifndef IP_ADDRESS_HPP
#define IP_ADDRESS_HPP

#include <cstdint>
#include "enum.hpp"
#include "types.hpp"

namespace netsock {
    class ip_address {
    public:
        ip_address();
        ip_address(const std::string &address);
        ip_address(const char *address);
        ip_address(const std::string &address, uint32_t scope);
        ip_address(const char *address, uint32_t scope);
        ip_address(addrv4_t address);
        ip_address(addrv6_t address, uint32_t scope);

        [[nodiscard]] address_family family() const;
        [[nodiscard]] addrv4_t addr() const;
        [[nodiscard]] uint32_t scope() const;
        [[nodiscard]] const uint16_t *addr6() const;

        static const ip_address zero;
        static const ip_address any;
        static const ip_address loopback;
        static const ip_address broadcast;

        [[nodiscard]] bool operator==(const ip_address &address) const;
        [[nodiscard]] bool operator!=(const ip_address &address) const;
    private:
        address_family _family;
        union {
            addrv4_t _address = 0;
            uint32_t _scope;
        };
        addrv6_t _bytes = {};
    };
}

#endif //IP_ADDRESS_HPP
