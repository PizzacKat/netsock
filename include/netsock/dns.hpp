#ifndef NETSOCK_DNS_HPP
#define NETSOCK_DNS_HPP

#include <vector>
#include "ip_address.hpp"
#include "error.h"

namespace netsock::dns {
    std::vector<ip_address> getHostAddresses(const std::string &hostname);

    std::vector<ip_address> getHostAddresses(const std::string &hostname, address_family family);
}

#endif //NETSOCK_DNS_HPP
