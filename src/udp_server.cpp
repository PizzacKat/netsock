#include <netsock/udp/udp_server.hpp>

namespace netsock {
    udp_server::udp_server(const std::string &address, unsigned short port): udp_server(ip_endpoint(ip_address(address), port)) {

    }

    udp_server::udp_server(const netsock::ip_endpoint &endpoint): m_server(socket_type::dgram, ip_protocol::udp) {
        m_server.bind(endpoint);
    }

    udp_server::udp_server(netsock::address_family family, const netsock::ip_endpoint &endpoint): m_server(family, socket_type::dgram, ip_protocol::udp) {
        m_server.bind(endpoint);
    }

    const socket &udp_server::server() const {
        return m_server;
    }

    socket &udp_server::server() {
        return m_server;
    }

    retsize_t udp_server::write_to(const netsock::ip_endpoint &endpoint, const char *data, netsock::datsize_t length, size_t offset) {
        return m_server.write_to(endpoint, data, length, offset);
    }

    size_t udp_server::write_to(const netsock::ip_endpoint &endpoint, const std::vector<char> &data) {
        return m_server.write_to(endpoint, data);
    }

    retsize_t udp_server::read_from(netsock::ip_endpoint &endpoint, char *out, netsock::datsize_t amount, size_t offset) {
        return m_server.read_from(endpoint, out, amount, offset);
    }

    std::vector<char> udp_server::read_from(netsock::ip_endpoint &endpoint, size_t amount) {
        return m_server.read_from(endpoint, amount);
    }

    void udp_server::close() {
        m_server.close();
    }

    udp_server::~udp_server() {
        close();
    }
}