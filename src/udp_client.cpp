#include <utility>

#include "netsock/udp/udp_client.hpp"
#include "netsock/dns.hpp"
#include "netsock/errors.hpp"

namespace netsock {
    udp_client::udp_client(): m_client(invalid_socket){

    }

    udp_client::udp_client(netsock::address_family family): m_client(family, socket_type::dgram, ip_protocol::udp) {

    }

    udp_client::udp_client(unsigned short port): udp_client(netsock::socket(socket_type::dgram, ip_protocol::udp)) {
        client().bind(ip_endpoint(ip_address("0.0.0.0"), port));
    }

    udp_client::udp_client(socket &&client): m_client(std::move(client)) {

    }

    void udp_client::connect(const netsock::ip_address &address, unsigned short port) {
        client().connect(address, port);
    }

    void udp_client::connect(const std::vector<ip_address> &addresses, unsigned short port) {
        client().connect(addresses, port);
    }

    void udp_client::connect(const std::string &address, unsigned short port) {
        auto addresses = dns::getHostAddresses(address);
        connect(addresses, port);
    }

    [[nodiscard]] bool udp_client::connected() const {
        return client().valid() && client().connected();
    }

    bool udp_client::pending() {
        return client().poll(0, netsock::poll_mode::read);
    }

    const socket &udp_client::client() const {
        return m_client;
    }

    socket &udp_client::client() {
        return m_client;
    }

    retsize_t udp_client::write(const char *data, netsock::datsize_t length, size_t offset) {
        return client().write(data, length, offset);
    }

    size_t udp_client::write(const std::vector<char> &data) {
        return client().write(data);
    }

    retsize_t udp_client::read(char *out, netsock::datsize_t amount, size_t offset) {
        return client().read(out, amount, offset);
    }

    std::vector<char> udp_client::read(size_t amount) {
        return client().read(amount);
    }

    retsize_t udp_client::write_to(const ip_endpoint &endpoint, const char *data, netsock::datsize_t length, size_t offset) {
        return client().write_to(endpoint, data, length, offset);
    }

    size_t udp_client::write_to(const ip_endpoint &endpoint, const std::vector<char> &data) {
        return client().write_to(endpoint, data);
    }

    retsize_t udp_client::write_to(const char *data, netsock::datsize_t length, size_t offset) {
        return client().write_to(data, length, offset);
    }

    size_t udp_client::write_to(const std::vector<char> &data) {
        return client().write_to(data);
    }

    retsize_t udp_client::read_from(ip_endpoint &endpoint, char *out, netsock::datsize_t amount, size_t offset) {
        return client().read_from(endpoint, out, amount, offset);
    }

    std::vector<char> udp_client::read_from(ip_endpoint &endpoint, size_t amount) {
        return client().read_from(endpoint, amount);
    }

    retsize_t udp_client::read_from(char *out, netsock::datsize_t amount, size_t offset) {
        return client().read_from(out, amount, offset);
    }

    std::vector<char> udp_client::read_from(size_t amount) {
        return client().read_from(amount);
    }

    void udp_client::close(){
        if (connected())
            m_client.shutdown();
        m_client.close();
    }

    udp_client udp_client::create() {
        return udp_client(socket(socket_type::dgram, ip_protocol::udp));
    }

    udp_client::~udp_client() {
        close();
    }
}