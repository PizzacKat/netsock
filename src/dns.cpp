#include "netsock/dns.hpp"

#include "netsock/impl/impl.hpp"
#include "netsock/errors.hpp"

namespace netsock::dns {
    std::vector<ip_address> getHostAddresses(const std::string &hostname) {
        addrinfo *info = nullptr;
        impl::result_t res = impl::getaddrinfo(hostname.c_str(), nullptr, nullptr, &info);
        if (res != 0)
            throw socket_exception(res, "getaddrinfo");
        std::vector<ip_address> addresses;
        for (auto *p = info; p != nullptr; p = p->ai_next){
            ip_address address;
            if (p->ai_family == (impl::addr_family_t)address_family::inet)
                address = ip_address::parse((char *)&((sockaddr_in *)p->ai_addr)->sin_addr.s_addr, 4);
            else {
                address = ip_address::parse((char *)&((sockaddr_in6 *)p->ai_addr)->sin6_addr, 16);
                address.set_scope_id(((sockaddr_in6 *) p->ai_addr)->sin6_scope_id);
            }
            addresses.push_back(address);
        }
        return addresses;
    }

    std::vector<ip_address> getHostAddresses(const std::string &hostname, address_family family) {
        addrinfo *info = nullptr;
        addrinfo hints{};
        hints.ai_family = (impl::addr_family_t)family;
        impl::result_t res = impl::getaddrinfo(hostname.c_str(), nullptr, &hints, &info);
        if (res != 0){
            if (res == socket_error::AFNOSUPPORT)
                throw std::invalid_argument("Unsupported family");
            throw socket_exception(res ,"getaddrinfo");
        }
        std::vector<ip_address> addresses;
        for (auto *p = info; p != nullptr; p = p->ai_next){
            ip_address address;
            if (p->ai_family == (impl::addr_family_t)address_family::inet)
                address = ip_address::parse((char *)&((sockaddr_in *)p->ai_addr)->sin_addr, 4);
            else {
                address = ip_address::parse((char *)&((sockaddr_in6 *)p->ai_addr)->sin6_addr, 16);
                address.set_scope_id(((sockaddr_in6 *) p->ai_addr)->sin6_scope_id);
            }
            addresses.push_back(address);
        }
        return addresses;
    }
}