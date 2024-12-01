#ifndef netsock_dns
#define netsock_dns

#include "impl.hpp"

namespace netsock::dns {
    inline std::vector<ip_address> resolve(const std::string &hostname) {
        auto addresses = impl::resolve_hosts(hostname);
        std::vector<ip_address> res(addresses.size());
        for (const auto &addr : addresses) {
            auto endpoint = impl::convert_address(addr);
            res.emplace_back(endpoint.address());
        }
        return res;
    }
}

#endif //netsock_dns
