#ifndef netsock_dns
#define netsock_dns

#include "impl.hpp"
#include <functional>
#include <optional>

namespace netsock::dns {
    inline std::vector<ip_address> resolve(const std::string &hostname) {
    std::vector addresses = impl::resolve_hosts(hostname);
        std::vector<ip_address> res(addresses.size());
        for (const auto &addr : addresses) {
            ip_address address = impl::convert_address(addr).address();
            res.emplace_back();
        }
        return res;
    }

    template <typename Pred>
    inline std::optional<ip_address> resolve_first(const std::string &hostname, Pred &&predicate) {
    std::vector addresses = impl::resolve_hosts(hostname);
        for (const auto &addr : addresses) {
            ip_address address = impl::convert_address(addr).address();
            if (std::invoke(predicate, address))
                return std::move(address);
        }
        return std::nullopt;
    }
}

#endif //netsock_dns
