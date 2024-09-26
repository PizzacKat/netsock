#include "netsock/ip_endpoint.hpp"
#include "netsock/impl.hpp"

namespace netsock {
    ip_endpoint::ip_endpoint(): _port(0) {

    }

    ip_endpoint::ip_endpoint(const ip_address &address, const uint16_t port): _address(address), _port(port) {

    }

    ip_endpoint::ip_endpoint(const std::string &ip) {
        if (const std::size_t find = ip.find('['); find != std::string::npos) {
            const std::size_t end = ip.find(']');
            if (end == std::string::npos)
                throw std::invalid_argument("invalid ip address");
            _address = ip.substr(find + 1, end - find - 1);
            const std::size_t separator = ip.find(':', end + 1);
            if (separator == std::string::npos)
                throw std::invalid_argument("invalid ip address");
            _port = std::stoi(ip.substr(separator + 1));
            return;
        }
        const std::size_t separator = ip.find(':');
        if (separator == std::string::npos) {
            _address = ip;
            _port = 0;
            return;
        }
        _address = ip.substr(0, separator);
        _port = std::stoi(ip.substr(separator + 1));
    }

    ip_endpoint::ip_endpoint(const char *ip): ip_endpoint(std::string(ip)) {

    }


    const ip_address &ip_endpoint::address() const {
        return _address;
    }

    uint16_t ip_endpoint::port() const {
        return _port;
    }

    ip_endpoint::operator socket_address() const {
        return impl::impl->convert_address(*this);
    }

}