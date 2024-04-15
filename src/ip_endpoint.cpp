#include "netsock/ip_endpoint.hpp"
#include <iostream>

namespace netsock {
    ip_endpoint::ip_endpoint()= default;

    ip_endpoint::ip_endpoint(const netsock::ip_address &address, unsigned short port) : address(address), port(port) {

    }

    ip_endpoint::ip_endpoint(const netsock::socket_address &address) {
        if (address.family() == inet){
            auto *addr = (sockaddr_in *)address.data();
            this->address = ip_address::parse((char *)&addr->sin_addr, 4);
            this->port = ip_address::network_to_host(addr->sin_port);
        } else {
            auto *addr = (sockaddr_in6 *)address.data();
            this->address = ip_address::parse((char *)&addr->sin6_addr, 16);
            this->address.set_scope_id(addr->sin6_scope_id);
            this->port = ip_address::network_to_host(addr->sin6_port);
        }
    }

    socket_address ip_endpoint::serialize() const {
        socket_address address(family());
        if (family() == inet){
            auto *addr = (sockaddr_in *)address.data();
            int size = 4;
            this->address.to_bytes((char *) &addr->sin_addr, size);
            addr->sin_port = ip_address::host_to_network(this->port);
        } else {
            auto *addr = (sockaddr_in6 *)address.data();
            int size = 16;
            this->address.to_bytes((char *) &addr->sin6_addr, size);
            addr->sin6_port = ip_address::host_to_network(this->port);
            addr->sin6_scope_id = ip_address::host_to_network(this->address.scopeId());
        }
        return address;
    }

    address_family ip_endpoint::family() const {
        return address.family();
    }

    ip_endpoint ip_endpoint::parse(const std::string &string) {
        auto separator = string.find_last_of(':');
        unsigned short port = std::stoi(string.substr(separator + 1));
        if (string[0] == '[' && string[separator - 1] == ']')
            return {ip_address(string.substr(1, separator - 2)), port};
        return {ip_address(string.substr(0, separator)), port};
    }

    bool ip_endpoint::operator==(const ip_endpoint &endpoint) const {
        return endpoint.address == address && endpoint.port == port;
    }
}