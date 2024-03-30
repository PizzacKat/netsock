#include <utility>

#include "netsock/udp/udp_socket.hpp"
#include "netsock/dns.hpp"
#include "netsock/errors.hpp"

namespace netsock {
    udp_socket::udp_socket(): m_socket(invalid_socket){

    }

    udp_socket::udp_socket(netsock::address_family family): m_socket(family, socket_type::dgram, ip_protocol::udp) {

    }

    udp_socket::udp_socket(unsigned short port): udp_socket(ip_endpoint(ip_address("0.0.0.0"), port)) {

    }

    udp_socket::udp_socket(const ip_endpoint &endpoint): udp_socket(netsock::socket(socket_type::dgram, ip_protocol::udp)) {
        socket().bind(endpoint);
    }

    udp_socket::udp_socket(netsock::socket &&socket): m_socket(std::move(socket)) {

    }

    udp_socket::udp_socket(netsock::udp_socket &&) noexcept= default;

    udp_socket &udp_socket::operator=(netsock::udp_socket &&connection) noexcept {
        if (m_socket.valid())
            m_socket.close();
        m_socket = std::move(connection.socket());
        return *this;
    }

    void udp_socket::connect(const netsock::ip_address &address, unsigned short port) {
        socket().connect(address, port);
    }

    void udp_socket::connect(const std::vector<ip_address> &addresses, unsigned short port) {
        socket().connect(addresses, port);
    }

    void udp_socket::connect(const std::string &address, unsigned short port) {
        auto addresses = dns::getHostAddresses(address);
        connect(addresses, port);
    }

    [[nodiscard]] bool udp_socket::connected() const {
        return socket().valid() && socket().connected();
    }

    bool udp_socket::pending() {
        return socket().poll(0, netsock::poll_mode::read);
    }

    const netsock::socket &udp_socket::socket() const {
        return m_socket;
    }

    netsock::socket &udp_socket::socket() {
        return m_socket;
    }

    retsize_t udp_socket::write(const char *data, netsock::datsize_t length, size_t offset) {
        return socket().write(data, length, offset);
    }

    size_t udp_socket::write(const std::vector<char> &data) {
        return socket().write(data);
    }

    retsize_t udp_socket::read(char *out, netsock::datsize_t amount, size_t offset) {
        return socket().read(out, amount, offset);
    }

    std::vector<char> udp_socket::read(size_t amount) {
        return socket().read(amount);
    }

    retsize_t udp_socket::write_to(const ip_endpoint &endpoint, const char *data, netsock::datsize_t length, size_t offset) {
        return socket().write_to(endpoint, data, length, offset);
    }

    size_t udp_socket::write_to(const ip_endpoint &endpoint, const std::vector<char> &data) {
        return socket().write_to(endpoint, data);
    }

    retsize_t udp_socket::read_from(ip_endpoint &endpoint, char *out, netsock::datsize_t amount, size_t offset) {
        return socket().read_from(endpoint, out, amount, offset);
    }

    std::vector<char> udp_socket::read_from(ip_endpoint &endpoint, size_t amount) {
        return socket().read_from(endpoint, amount);
    }

    void udp_socket::close(){
        if (connected())
            m_socket.shutdown();
        m_socket.close();
    }

    udp_socket udp_socket::create() {
        return udp_socket(netsock::socket(socket_type::dgram, ip_protocol::udp));
    }

    udp_socket::~udp_socket() {
        close();
    }
}