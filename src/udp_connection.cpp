#include <utility>

#include "netsock/udp/udp_connection.hpp"
#include "netsock/dns.hpp"
#include "netsock/errors.hpp"

namespace netsock {
    udp_connection::udp_connection(): m_socket(invalid_socket){

    }

    udp_connection::udp_connection(netsock::address_family family): m_socket(family, socket_type::dgram, ip_protocol::udp) {

    }

    udp_connection::udp_connection(unsigned short port): udp_connection(ip_endpoint(ip_address("0.0.0.0"), port)) {

    }

    udp_connection::udp_connection(const ip_endpoint &endpoint): udp_connection(netsock::socket(socket_type::dgram, ip_protocol::udp)) {
        socket().bind(endpoint);
    }

    udp_connection::udp_connection(netsock::socket &&socket): m_socket(std::move(socket)) {

    }

    udp_connection::udp_connection(netsock::udp_connection &&) noexcept= default;

    udp_connection &udp_connection::operator=(netsock::udp_connection &&connection) noexcept {
        if (m_socket.valid())
            m_socket.close();
        m_socket = std::move(connection.socket());
        return *this;
    }

    void udp_connection::connect(const netsock::ip_address &address, unsigned short port) {
        socket().connect(address, port);
    }

    void udp_connection::connect(const std::vector<ip_address> &addresses, unsigned short port) {
        socket().connect(addresses, port);
    }

    void udp_connection::connect(const std::string &address, unsigned short port) {
        auto addresses = dns::getHostAddresses(address);
        connect(addresses, port);
    }

    [[nodiscard]] bool udp_connection::connected() const {
        return socket().valid() && socket().connected();
    }

    bool udp_connection::pending() {
        return socket().poll(0, netsock::poll_mode::read);
    }

    const netsock::socket &udp_connection::socket() const {
        return m_socket;
    }

    netsock::socket &udp_connection::socket() {
        return m_socket;
    }

    retsize_t udp_connection::write(const char *data, netsock::datsize_t length, size_t offset) {
        return socket().write(data, length, offset);
    }

    size_t udp_connection::write(const std::vector<char> &data) {
        return socket().write(data);
    }

    retsize_t udp_connection::read(char *out, netsock::datsize_t amount, size_t offset) {
        return socket().read(out, amount, offset);
    }

    std::vector<char> udp_connection::read(size_t amount) {
        return socket().read(amount);
    }

    retsize_t udp_connection::write_to(const ip_endpoint &endpoint, const char *data, netsock::datsize_t length, size_t offset) {
        return socket().write_to(endpoint, data, length, offset);
    }

    size_t udp_connection::write_to(const ip_endpoint &endpoint, const std::vector<char> &data) {
        return socket().write_to(endpoint, data);
    }

    retsize_t udp_connection::read_from(ip_endpoint &endpoint, char *out, netsock::datsize_t amount, size_t offset) {
        return socket().read_from(endpoint, out, amount, offset);
    }

    std::vector<char> udp_connection::read_from(ip_endpoint &endpoint, size_t amount) {
        return socket().read_from(endpoint, amount);
    }

    void udp_connection::close(){
        if (connected())
            m_socket.shutdown();
        m_socket.close();
    }

    udp_connection udp_connection::create() {
        return udp_connection(netsock::socket(socket_type::dgram, ip_protocol::udp));
    }

    udp_connection::~udp_connection() {
        close();
    }
}