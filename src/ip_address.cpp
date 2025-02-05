#include "netsock/ip_address.hpp"

#include <algorithm>

#include "netsock/impl.hpp"

namespace netsock {
    ip_address::ip_address(): _family(af_inet) {

    }

    ip_address::ip_address(const addrv4_t address): _family(af_inet), _address(address) {

    }

    ip_address::ip_address(const std::string &address) {
        if (address.find(':') != std::string::npos) {
            _family = af_inet6;
            impl::parse_address(address, _bytes);
            return;
        }
        _family = af_inet;
        impl::parse_address(address, _address);
    }

    ip_address::ip_address(const char *address): ip_address(std::string(address)) {

    }

    ip_address::ip_address(addrv6_t address, const uint32_t scope): _family(af_inet6) {
        std::copy_n(address, 8, _bytes);
        _scope = scope;
    }

    ip_address::ip_address(const std::string &address, const uint32_t scope): _family(af_inet6) {
        impl::parse_address(address, _bytes);
        _scope = scope;
    }

    ip_address::ip_address(const char *address, const uint32_t scope): ip_address(std::string(address)) {

    }

    address_family ip_address::family() const {
        return _family;
    }

    addrv4_t ip_address::addr() const {
        return _address;
    }

    uint32_t ip_address::scope() const {
        return _scope;
    }

    const uint16_t *ip_address::addr6() const {
        return _bytes;
    }

    bool ip_address::operator==(const ip_address &address) const {
        if (address.family() != family())
            return false;
        if (family() == af_inet)
            return addr() == address.addr();
        for (std::size_t i = 0; i < 8; i++)
            if (addr6()[i] != address.addr6()[i])
                return false;
        return true;
    }

    bool ip_address::operator!=(const ip_address &address) const {
        return !(*this == address);
    }

    const ip_address ip_address::zero(0x00000000u);
    const ip_address ip_address::any(0x00000000u);
    const ip_address ip_address::loopback(0x7F000001u);
    const ip_address ip_address::broadcast(0xFFFFFFFFu);
}
