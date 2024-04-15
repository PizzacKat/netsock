#ifndef NETSOCK_DNS_HPP
#define NETSOCK_DNS_HPP

#if defined(__WIN32__) || defined(__WIN64__)
#include <ws2tcpip.h>
#elif defined(__unix__)
#include <netdb.h>
#endif
#include <vector>
#include "ip_address.hpp"
#include "error.h"

namespace netsock::dns {
    static inline std::vector<ip_address> getHostAddresses(const std::string &hostname){
        addrinfo *info = nullptr;
        int res = getaddrinfo(hostname.c_str(), nullptr, nullptr, &info);
        if (res != 0)
            throw socket_exception(res, "getaddrinfo");
        std::vector<ip_address> addresses;
        for (auto *p = info; p != nullptr; p = p->ai_next){
            ip_address address;
            if (p->ai_family == inet)
                address = ip_address::parse((char *)&((sockaddr_in *)p->ai_addr)->sin_addr.s_addr, 4);
            else {
                address = ip_address::parse((char *)&((sockaddr_in6 *)p->ai_addr)->sin6_addr, 16);
                address.set_scope_id(((sockaddr_in6 *) p->ai_addr)->sin6_scope_id);
            }
            addresses.push_back(address);
        }
        return addresses;
    }

    static inline std::vector<ip_address> getHostAddresses(const std::string &hostname, address_family family){
        addrinfo *info = nullptr;
        addrinfo hints{};
        hints.ai_family = family;
        int res = getaddrinfo(hostname.c_str(), nullptr, &hints, &info);
        if (res != 0){
            if (res == socket_error::AFNOSUPPORT)
                throw std::invalid_argument("Unsupported family");
            throw socket_exception(res ,"getaddrinfo");
        }
        std::vector<ip_address> addresses;
        for (auto *p = info; p != nullptr; p = p->ai_next){
            ip_address address;
            if (p->ai_family == inet)
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

#endif //NETSOCK_DNS_HPP
